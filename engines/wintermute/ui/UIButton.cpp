/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/dcgf.h"
#include "engines/wintermute/base/BDynBuffer.h"
#include "engines/wintermute/base/BGame.h"
#include "engines/wintermute/ui/UIButton.h"
#include "engines/wintermute/ui/UITiledImage.h"
#include "engines/wintermute/base/BParser.h"
#include "engines/wintermute/base/BActiveRect.h"
#include "engines/wintermute/base/font/BFontStorage.h"
#include "engines/wintermute/base/font/BFont.h"
#include "engines/wintermute/base/BStringTable.h"
#include "engines/wintermute/base/BSprite.h"
#include "engines/wintermute/base/BFileManager.h"
#include "engines/wintermute/platform_osystem.h"
#include "engines/wintermute/base/scriptables/ScValue.h"
#include "engines/wintermute/base/scriptables/ScScript.h"
#include "engines/wintermute/base/scriptables/ScStack.h"

namespace WinterMute {

IMPLEMENT_PERSISTENT(CUIButton, false)

//////////////////////////////////////////////////////////////////////////
CUIButton::CUIButton(CBGame *inGame): CUIObject(inGame) {
	_backPress = _backHover = _backDisable = _backFocus = NULL;

	_fontHover = _fontPress = _fontDisable = _fontFocus = NULL;

	_imageDisable = _imagePress = _imageHover = _imageFocus = NULL;

	_align = TAL_CENTER;

	_hover = _press = false;

	_type = UI_BUTTON;

	_canFocus = false;
	_stayPressed = false;

	_oneTimePress = false;
	_centerImage = false;

	_pixelPerfect = false;
}


//////////////////////////////////////////////////////////////////////////
CUIButton::~CUIButton() {
	delete _backPress;
	delete _backHover;
	delete _backDisable;
	delete _backFocus;

	if (!_sharedFonts) {
		if (_fontHover)   _gameRef->_fontStorage->removeFont(_fontHover);
		if (_fontPress)   _gameRef->_fontStorage->removeFont(_fontPress);
		if (_fontDisable) _gameRef->_fontStorage->removeFont(_fontDisable);
		if (_fontFocus)   _gameRef->_fontStorage->removeFont(_fontFocus);
	}

	if (!_sharedImages) {
		delete _imageHover;
		delete _imagePress;
		delete _imageDisable;
		delete _imageFocus;
	}
}


//////////////////////////////////////////////////////////////////////////
bool CUIButton::loadFile(const char *filename) {
	byte *buffer = _gameRef->_fileManager->readWholeFile(filename);
	if (buffer == NULL) {
		_gameRef->LOG(0, "CUIButton::LoadFile failed for file '%s'", filename);
		return STATUS_FAILED;
	}

	bool ret;

	_filename = new char [strlen(filename) + 1];
	strcpy(_filename, filename);

	if (DID_FAIL(ret = loadBuffer(buffer, true))) _gameRef->LOG(0, "Error parsing BUTTON file '%s'", filename);

	delete [] buffer;

	return ret;
}


TOKEN_DEF_START
TOKEN_DEF(BUTTON)
TOKEN_DEF(TEMPLATE)
TOKEN_DEF(DISABLED)
TOKEN_DEF(VISIBLE)
TOKEN_DEF(FOCUSABLE)
TOKEN_DEF(BACK_HOVER)
TOKEN_DEF(BACK_PRESS)
TOKEN_DEF(BACK_DISABLE)
TOKEN_DEF(BACK_FOCUS)
TOKEN_DEF(BACK)
TOKEN_DEF(CENTER_IMAGE)
TOKEN_DEF(IMAGE_HOVER)
TOKEN_DEF(IMAGE_PRESS)
TOKEN_DEF(IMAGE_DISABLE)
TOKEN_DEF(IMAGE_FOCUS)
TOKEN_DEF(IMAGE)
TOKEN_DEF(FONT_HOVER)
TOKEN_DEF(FONT_PRESS)
TOKEN_DEF(FONT_DISABLE)
TOKEN_DEF(FONT_FOCUS)
TOKEN_DEF(FONT)
TOKEN_DEF(TEXT_ALIGN)
TOKEN_DEF(TEXT)
TOKEN_DEF(X)
TOKEN_DEF(Y)
TOKEN_DEF(WIDTH)
TOKEN_DEF(HEIGHT)
TOKEN_DEF(CURSOR)
TOKEN_DEF(NAME)
TOKEN_DEF(EVENTS)
TOKEN_DEF(SCRIPT)
TOKEN_DEF(CAPTION)
TOKEN_DEF(PARENT_NOTIFY)
TOKEN_DEF(PRESSED)
TOKEN_DEF(PIXEL_PERFECT)
TOKEN_DEF(EDITOR_PROPERTY)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
bool CUIButton::loadBuffer(byte *buffer, bool complete) {
	TOKEN_TABLE_START(commands)
	TOKEN_TABLE(BUTTON)
	TOKEN_TABLE(TEMPLATE)
	TOKEN_TABLE(DISABLED)
	TOKEN_TABLE(VISIBLE)
	TOKEN_TABLE(FOCUSABLE)
	TOKEN_TABLE(BACK_HOVER)
	TOKEN_TABLE(BACK_PRESS)
	TOKEN_TABLE(BACK_DISABLE)
	TOKEN_TABLE(BACK_FOCUS)
	TOKEN_TABLE(BACK)
	TOKEN_TABLE(CENTER_IMAGE)
	TOKEN_TABLE(IMAGE_HOVER)
	TOKEN_TABLE(IMAGE_PRESS)
	TOKEN_TABLE(IMAGE_DISABLE)
	TOKEN_TABLE(IMAGE_FOCUS)
	TOKEN_TABLE(IMAGE)
	TOKEN_TABLE(FONT_HOVER)
	TOKEN_TABLE(FONT_PRESS)
	TOKEN_TABLE(FONT_DISABLE)
	TOKEN_TABLE(FONT_FOCUS)
	TOKEN_TABLE(FONT)
	TOKEN_TABLE(TEXT_ALIGN)
	TOKEN_TABLE(TEXT)
	TOKEN_TABLE(X)
	TOKEN_TABLE(Y)
	TOKEN_TABLE(WIDTH)
	TOKEN_TABLE(HEIGHT)
	TOKEN_TABLE(CURSOR)
	TOKEN_TABLE(NAME)
	TOKEN_TABLE(EVENTS)
	TOKEN_TABLE(SCRIPT)
	TOKEN_TABLE(CAPTION)
	TOKEN_TABLE(PARENT_NOTIFY)
	TOKEN_TABLE(PRESSED)
	TOKEN_TABLE(PIXEL_PERFECT)
	TOKEN_TABLE(EDITOR_PROPERTY)
	TOKEN_TABLE_END

	byte *params;
	int cmd = 2;
	CBParser parser(_gameRef);

	if (complete) {
		if (parser.getCommand((char **)&buffer, commands, (char **)&params) != TOKEN_BUTTON) {
			_gameRef->LOG(0, "'BUTTON' keyword expected.");
			return STATUS_FAILED;
		}
		buffer = params;
	}

	while (cmd > 0 && (cmd = parser.getCommand((char **)&buffer, commands, (char **)&params)) > 0) {
		switch (cmd) {
		case TOKEN_TEMPLATE:
			if (DID_FAIL(loadFile((char *)params))) cmd = PARSERR_GENERIC;
			break;

		case TOKEN_NAME:
			setName((char *)params);
			break;

		case TOKEN_CAPTION:
			setCaption((char *)params);
			break;

		case TOKEN_BACK:
			delete _back;
			_back = new CUITiledImage(_gameRef);
			if (!_back || DID_FAIL(_back->loadFile((char *)params))) {
				delete _back;
				_back = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_BACK_HOVER:
			delete _backHover;
			_backHover = new CUITiledImage(_gameRef);
			if (!_backHover || DID_FAIL(_backHover->loadFile((char *)params))) {
				delete _backHover;
				_backHover = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_BACK_PRESS:
			delete _backPress;
			_backPress = new CUITiledImage(_gameRef);
			if (!_backPress || DID_FAIL(_backPress->loadFile((char *)params))) {
				delete _backPress;
				_backPress = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_BACK_DISABLE:
			delete _backDisable;
			_backDisable = new CUITiledImage(_gameRef);
			if (!_backDisable || DID_FAIL(_backDisable->loadFile((char *)params))) {
				delete _backDisable;
				_backDisable = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_BACK_FOCUS:
			delete _backFocus;
			_backFocus = new CUITiledImage(_gameRef);
			if (!_backFocus || DID_FAIL(_backFocus->loadFile((char *)params))) {
				delete _backFocus;
				_backFocus = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_IMAGE:
			delete _image;
			_image = new CBSprite(_gameRef);
			if (!_image || DID_FAIL(_image->loadFile((char *)params))) {
				delete _image;
				_image = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_IMAGE_HOVER:
			delete _imageHover;
			_imageHover = new CBSprite(_gameRef);
			if (!_imageHover || DID_FAIL(_imageHover->loadFile((char *)params))) {
				delete _imageHover;
				_imageHover = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_IMAGE_PRESS:
			delete _imagePress;
			_imagePress = new CBSprite(_gameRef);
			if (!_imagePress || DID_FAIL(_imagePress->loadFile((char *)params))) {
				delete _imagePress;
				_imagePress = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_IMAGE_DISABLE:
			delete _imageDisable;
			_imageDisable = new CBSprite(_gameRef);
			if (!_imageDisable || DID_FAIL(_imageDisable->loadFile((char *)params))) {
				delete _imageDisable;
				_imageDisable = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_IMAGE_FOCUS:
			delete _imageFocus;
			_imageFocus = new CBSprite(_gameRef);
			if (!_imageFocus || DID_FAIL(_imageFocus->loadFile((char *)params))) {
				delete _imageFocus;
				_imageFocus = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_FONT:
			if (_font) _gameRef->_fontStorage->removeFont(_font);
			_font = _gameRef->_fontStorage->addFont((char *)params);
			if (!_font) cmd = PARSERR_GENERIC;
			break;

		case TOKEN_FONT_HOVER:
			if (_fontHover) _gameRef->_fontStorage->removeFont(_fontHover);
			_fontHover = _gameRef->_fontStorage->addFont((char *)params);
			if (!_fontHover) cmd = PARSERR_GENERIC;
			break;

		case TOKEN_FONT_PRESS:
			if (_fontPress) _gameRef->_fontStorage->removeFont(_fontPress);
			_fontPress = _gameRef->_fontStorage->addFont((char *)params);
			if (!_fontPress) cmd = PARSERR_GENERIC;
			break;

		case TOKEN_FONT_DISABLE:
			if (_fontDisable) _gameRef->_fontStorage->removeFont(_fontDisable);
			_fontDisable = _gameRef->_fontStorage->addFont((char *)params);
			if (!_fontDisable) cmd = PARSERR_GENERIC;
			break;

		case TOKEN_FONT_FOCUS:
			if (_fontFocus) _gameRef->_fontStorage->removeFont(_fontFocus);
			_fontFocus = _gameRef->_fontStorage->addFont((char *)params);
			if (!_fontFocus) cmd = PARSERR_GENERIC;
			break;

		case TOKEN_TEXT:
			setText((char *)params);
			_gameRef->_stringTable->expand(&_text);
			break;

		case TOKEN_TEXT_ALIGN:
			if (scumm_stricmp((char *)params, "left") == 0) _align = TAL_LEFT;
			else if (scumm_stricmp((char *)params, "right") == 0) _align = TAL_RIGHT;
			else _align = TAL_CENTER;
			break;

		case TOKEN_X:
			parser.scanStr((char *)params, "%d", &_posX);
			break;

		case TOKEN_Y:
			parser.scanStr((char *)params, "%d", &_posY);
			break;

		case TOKEN_WIDTH:
			parser.scanStr((char *)params, "%d", &_width);
			break;

		case TOKEN_HEIGHT:
			parser.scanStr((char *)params, "%d", &_height);
			break;

		case TOKEN_CURSOR:
			delete _cursor;
			_cursor = new CBSprite(_gameRef);
			if (!_cursor || DID_FAIL(_cursor->loadFile((char *)params))) {
				delete _cursor;
				_cursor = NULL;
				cmd = PARSERR_GENERIC;
			}
			break;

		case TOKEN_SCRIPT:
			addScript((char *)params);
			break;

		case TOKEN_PARENT_NOTIFY:
			parser.scanStr((char *)params, "%b", &_parentNotify);
			break;

		case TOKEN_DISABLED:
			parser.scanStr((char *)params, "%b", &_disable);
			break;

		case TOKEN_VISIBLE:
			parser.scanStr((char *)params, "%b", &_visible);
			break;

		case TOKEN_FOCUSABLE:
			parser.scanStr((char *)params, "%b", &_canFocus);
			break;

		case TOKEN_CENTER_IMAGE:
			parser.scanStr((char *)params, "%b", &_centerImage);
			break;

		case TOKEN_PRESSED:
			parser.scanStr((char *)params, "%b", &_stayPressed);
			break;

		case TOKEN_PIXEL_PERFECT:
			parser.scanStr((char *)params, "%b", &_pixelPerfect);
			break;

		case TOKEN_EDITOR_PROPERTY:
			parseEditorProperty(params, false);
			break;
		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND) {
		_gameRef->LOG(0, "Syntax error in BUTTON definition");
		return STATUS_FAILED;
	}
	if (cmd == PARSERR_GENERIC) {
		_gameRef->LOG(0, "Error loading BUTTON definition");
		return STATUS_FAILED;
	}

	correctSize();

	return STATUS_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CUIButton::saveAsText(CBDynBuffer *buffer, int indent) {
	buffer->putTextIndent(indent, "BUTTON\n");
	buffer->putTextIndent(indent, "{\n");

	buffer->putTextIndent(indent + 2, "NAME=\"%s\"\n", _name);
	buffer->putTextIndent(indent + 2, "CAPTION=\"%s\"\n", getCaption());

	buffer->putTextIndent(indent + 2, "\n");

	if (_back && _back->_filename)
		buffer->putTextIndent(indent + 2, "BACK=\"%s\"\n", _back->_filename);
	if (_backHover && _backHover->_filename)
		buffer->putTextIndent(indent + 2, "BACK_HOVER=\"%s\"\n", _backHover->_filename);
	if (_backPress && _backPress->_filename)
		buffer->putTextIndent(indent + 2, "BACK_PRESS=\"%s\"\n", _backPress->_filename);
	if (_backDisable && _backDisable->_filename)
		buffer->putTextIndent(indent + 2, "BACK_DISABLE=\"%s\"\n", _backDisable->_filename);
	if (_backFocus && _backFocus->_filename)
		buffer->putTextIndent(indent + 2, "BACK_FOCUS=\"%s\"\n", _backFocus->_filename);

	if (_image && _image->_filename)
		buffer->putTextIndent(indent + 2, "IMAGE=\"%s\"\n", _image->_filename);
	if (_imageHover && _imageHover->_filename)
		buffer->putTextIndent(indent + 2, "IMAGE_HOVER=\"%s\"\n", _imageHover->_filename);
	if (_imagePress && _imagePress->_filename)
		buffer->putTextIndent(indent + 2, "IMAGE_PRESS=\"%s\"\n", _imagePress->_filename);
	if (_imageDisable && _imageDisable->_filename)
		buffer->putTextIndent(indent + 2, "IMAGE_DISABLE=\"%s\"\n", _imageDisable->_filename);
	if (_imageFocus && _imageFocus->_filename)
		buffer->putTextIndent(indent + 2, "IMAGE_FOCUS=\"%s\"\n", _imageFocus->_filename);

	if (_font && _font->_filename)
		buffer->putTextIndent(indent + 2, "FONT=\"%s\"\n", _font->_filename);
	if (_fontHover && _fontHover->_filename)
		buffer->putTextIndent(indent + 2, "FONT_HOVER=\"%s\"\n", _fontHover->_filename);
	if (_fontPress && _fontPress->_filename)
		buffer->putTextIndent(indent + 2, "FONT_PRESS=\"%s\"\n", _fontPress->_filename);
	if (_fontDisable && _fontDisable->_filename)
		buffer->putTextIndent(indent + 2, "FONT_DISABLE=\"%s\"\n", _fontDisable->_filename);
	if (_fontFocus && _fontFocus->_filename)
		buffer->putTextIndent(indent + 2, "FONT_FOCUS=\"%s\"\n", _fontFocus->_filename);

	if (_cursor && _cursor->_filename)
		buffer->putTextIndent(indent + 2, "CURSOR=\"%s\"\n", _cursor->_filename);


	buffer->putTextIndent(indent + 2, "\n");

	if (_text)
		buffer->putTextIndent(indent + 2, "TEXT=\"%s\"\n", _text);

	switch (_align) {
	case TAL_LEFT:
		buffer->putTextIndent(indent + 2, "TEXT_ALIGN=\"%s\"\n", "left");
		break;
	case TAL_RIGHT:
		buffer->putTextIndent(indent + 2, "TEXT_ALIGN=\"%s\"\n", "right");
		break;
	case TAL_CENTER:
		buffer->putTextIndent(indent + 2, "TEXT_ALIGN=\"%s\"\n", "center");
		break;
	default:
		warning("CUIButton::SaveAsText - unhandled enum");
		break;
	}

	buffer->putTextIndent(indent + 2, "\n");

	buffer->putTextIndent(indent + 2, "X=%d\n", _posX);
	buffer->putTextIndent(indent + 2, "Y=%d\n", _posY);
	buffer->putTextIndent(indent + 2, "WIDTH=%d\n", _width);
	buffer->putTextIndent(indent + 2, "HEIGHT=%d\n", _height);


	buffer->putTextIndent(indent + 2, "DISABLED=%s\n", _disable ? "TRUE" : "FALSE");
	buffer->putTextIndent(indent + 2, "VISIBLE=%s\n", _visible ? "TRUE" : "FALSE");
	buffer->putTextIndent(indent + 2, "PARENT_NOTIFY=%s\n", _parentNotify ? "TRUE" : "FALSE");
	buffer->putTextIndent(indent + 2, "FOCUSABLE=%s\n", _canFocus ? "TRUE" : "FALSE");
	buffer->putTextIndent(indent + 2, "CENTER_IMAGE=%s\n", _centerImage ? "TRUE" : "FALSE");
	buffer->putTextIndent(indent + 2, "PRESSED=%s\n", _stayPressed ? "TRUE" : "FALSE");
	buffer->putTextIndent(indent + 2, "PIXEL_PERFECT=%s\n", _pixelPerfect ? "TRUE" : "FALSE");

	buffer->putTextIndent(indent + 2, "\n");

	// scripts
	for (int i = 0; i < _scripts.getSize(); i++) {
		buffer->putTextIndent(indent + 2, "SCRIPT=\"%s\"\n", _scripts[i]->_filename);
	}

	buffer->putTextIndent(indent + 2, "\n");

	// editor properties
	CBBase::saveAsText(buffer, indent + 2);

	buffer->putTextIndent(indent, "}\n");
	return STATUS_OK;
}

//////////////////////////////////////////////////////////////////////////
void CUIButton::correctSize() {
	Rect32 rect;

	CBSprite *img = NULL;
	if (_image) img = _image;
	else if (_imageDisable) img = _imageDisable;
	else if (_imageHover) img = _imageHover;
	else if (_imagePress) img = _imagePress;
	else if (_imageFocus) img = _imageFocus;

	if (_width <= 0) {
		if (img) {
			img->getBoundingRect(&rect, 0, 0);
			_width = rect.right - rect.left;
		} else _width = 100;
	}

	if (_height <= 0) {
		if (img) {
			img->getBoundingRect(&rect, 0, 0);
			_height = rect.bottom - rect.top;
		}
	}

	if (_text) {
		int text_height;
		if (_font) text_height = _font->getTextHeight((byte *)_text, _width);
		else text_height = _gameRef->_systemFont->getTextHeight((byte *)_text, _width);

		if (text_height > _height) _height = text_height;
	}

	if (_height <= 0) _height = 100;

	if (_back) _back->correctSize(&_width, &_height);
}


//////////////////////////////////////////////////////////////////////////
bool CUIButton::display(int offsetX, int offsetY) {
	if (!_visible)
		return STATUS_OK;

	CUITiledImage *back = NULL;
	CBSprite *image = NULL;
	CBFont *font = 0;

	//RECT rect;
	//CBPlatform::setRect(&rect, OffsetX + _posX, OffsetY + _posY, OffsetX+_posX+_width, OffsetY+_posY+_height);
	//_hover = (!_disable && CBPlatform::ptInRect(&rect, _gameRef->_mousePos)!=FALSE);
	_hover = (!_disable && _gameRef->_activeObject == this && (_gameRef->_interactive || _gameRef->_state == GAME_SEMI_FROZEN));

	if ((_press && _hover && !_gameRef->_mouseLeftDown) ||
	        (_oneTimePress && CBPlatform::getTime() - _oneTimePressTime >= 100)) press();


	if (_disable) {
		if (_backDisable) back = _backDisable;
		if (_imageDisable) image = _imageDisable;
		if (_text && _fontDisable) font = _fontDisable;
	} else if (_press || _oneTimePress || _stayPressed) {
		if (_backPress) back = _backPress;
		if (_imagePress) image = _imagePress;
		if (_text && _fontPress) font = _fontPress;
	} else if (_hover) {
		if (_backHover) back = _backHover;
		if (_imageHover) image = _imageHover;
		if (_text && _fontHover) font = _fontHover;
	} else if (_canFocus && isFocused()) {
		if (_backFocus) back = _backFocus;
		if (_imageFocus) image = _imageFocus;
		if (_text && _fontFocus) font = _fontFocus;
	}

	if (!back && _back) back = _back;
	if (!image && _image) image = _image;
	if (_text && !font) {
		if (_font) font = _font;
		else font = _gameRef->_systemFont;
	}

	int imageX = offsetX + _posX;
	int imageY = offsetY + _posY;

	if (image && _centerImage) {
		Rect32 rc;
		image->getBoundingRect(&rc, 0, 0);
		imageX += (_width - (rc.right - rc.left)) / 2;
		imageY += (_height - (rc.bottom - rc.top)) / 2;
	}

	if (back) back->display(offsetX + _posX, offsetY + _posY, _width, _height);
	//if(image) image->Draw(ImageX +((_press||_oneTimePress)&&back?1:0), ImageY +((_press||_oneTimePress)&&back?1:0), NULL);
	if (image) image->draw(imageX + ((_press || _oneTimePress) && back ? 1 : 0), imageY + ((_press || _oneTimePress) && back ? 1 : 0), _pixelPerfect ? this : NULL);

	if (font && _text) {
		int text_offset = (_height - font->getTextHeight((byte *)_text, _width)) / 2;
		font->drawText((byte *)_text, offsetX + _posX + ((_press || _oneTimePress) ? 1 : 0), offsetY + _posY + text_offset + ((_press || _oneTimePress) ? 1 : 0), _width, _align);
	}

	if (!_pixelPerfect || !_image) _gameRef->_renderer->_rectList.add(new CBActiveRect(_gameRef,  this, NULL, offsetX + _posX, offsetY + _posY, _width, _height, 100, 100, false));

	// reset unused sprites
	if (_image && _image != image) _image->reset();
	if (_imageDisable && _imageDisable != image) _imageDisable->reset();
	if (_imageFocus && _imageFocus != image) _imageFocus->reset();
	if (_imagePress && _imagePress != image) _imagePress->reset();
	if (_imageHover && _imageHover != image) _imageHover->reset();

	_press = _hover && _gameRef->_mouseLeftDown && _gameRef->_capturedObject == this;

	return STATUS_OK;
}


//////////////////////////////////////////////////////////////////////////
void CUIButton::press() {
	applyEvent("Press");
	if (_listenerObject) _listenerObject->listen(_listenerParamObject, _listenerParamDWORD);
	if (_parentNotify && _parent) _parent->applyEvent(_name);

	_oneTimePress = false;
}


//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
bool CUIButton::scCallMethod(CScScript *script, CScStack *stack, CScStack *thisStack, const char *name) {
	//////////////////////////////////////////////////////////////////////////
	// SetDisabledFont
	//////////////////////////////////////////////////////////////////////////
	if (strcmp(name, "SetDisabledFont") == 0) {
		stack->correctParams(1);
		CScValue *Val = stack->pop();

		if (_fontDisable) _gameRef->_fontStorage->removeFont(_fontDisable);
		if (Val->isNULL()) {
			_fontDisable = NULL;
			stack->pushBool(true);
		} else {
			_fontDisable = _gameRef->_fontStorage->addFont(Val->getString());
			stack->pushBool(_fontDisable != NULL);
		}
		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetHoverFont
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "SetHoverFont") == 0) {
		stack->correctParams(1);
		CScValue *val = stack->pop();

		if (_fontHover) _gameRef->_fontStorage->removeFont(_fontHover);
		if (val->isNULL()) {
			_fontHover = NULL;
			stack->pushBool(true);
		} else {
			_fontHover = _gameRef->_fontStorage->addFont(val->getString());
			stack->pushBool(_fontHover != NULL);
		}
		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetPressedFont
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "SetPressedFont") == 0) {
		stack->correctParams(1);
		CScValue *Val = stack->pop();

		if (_fontPress) _gameRef->_fontStorage->removeFont(_fontPress);
		if (Val->isNULL()) {
			_fontPress = NULL;
			stack->pushBool(true);
		} else {
			_fontPress = _gameRef->_fontStorage->addFont(Val->getString());
			stack->pushBool(_fontPress != NULL);
		}
		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetFocusedFont
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "SetFocusedFont") == 0) {
		stack->correctParams(1);
		CScValue *val = stack->pop();

		if (_fontFocus) _gameRef->_fontStorage->removeFont(_fontFocus);
		if (val->isNULL()) {
			_fontFocus = NULL;
			stack->pushBool(true);
		} else {
			_fontFocus = _gameRef->_fontStorage->addFont(val->getString());
			stack->pushBool(_fontFocus != NULL);
		}
		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetDisabledImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "SetDisabledImage") == 0) {
		stack->correctParams(1);

		delete _imageDisable;
		_imageDisable = new CBSprite(_gameRef);
		const char *filename = stack->pop()->getString();
		if (!_imageDisable || DID_FAIL(_imageDisable->loadFile(filename))) {
			delete _imageDisable;
			_imageDisable = NULL;
			stack->pushBool(false);
		} else stack->pushBool(true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetDisabledImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetDisabledImage") == 0) {
		stack->correctParams(0);
		if (!_imageDisable || !_imageDisable->_filename) stack->pushNULL();
		else stack->pushString(_imageDisable->_filename);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetDisabledImageObject
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetDisabledImageObject") == 0) {
		stack->correctParams(0);
		if (!_imageDisable) stack->pushNULL();
		else stack->pushNative(_imageDisable, true);

		return STATUS_OK;
	}


	//////////////////////////////////////////////////////////////////////////
	// SetHoverImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "SetHoverImage") == 0) {
		stack->correctParams(1);

		delete _imageHover;
		_imageHover = new CBSprite(_gameRef);
		const char *filename = stack->pop()->getString();
		if (!_imageHover || DID_FAIL(_imageHover->loadFile(filename))) {
			delete _imageHover;
			_imageHover = NULL;
			stack->pushBool(false);
		} else stack->pushBool(true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetHoverImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetHoverImage") == 0) {
		stack->correctParams(0);
		if (!_imageHover || !_imageHover->_filename) stack->pushNULL();
		else stack->pushString(_imageHover->_filename);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetHoverImageObject
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetHoverImageObject") == 0) {
		stack->correctParams(0);
		if (!_imageHover) stack->pushNULL();
		else stack->pushNative(_imageHover, true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetPressedImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "SetPressedImage") == 0) {
		stack->correctParams(1);

		delete _imagePress;
		_imagePress = new CBSprite(_gameRef);
		const char *filename = stack->pop()->getString();
		if (!_imagePress || DID_FAIL(_imagePress->loadFile(filename))) {
			delete _imagePress;
			_imagePress = NULL;
			stack->pushBool(false);
		} else stack->pushBool(true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetPressedImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetPressedImage") == 0) {
		stack->correctParams(0);
		if (!_imagePress || !_imagePress->_filename) stack->pushNULL();
		else stack->pushString(_imagePress->_filename);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetPressedImageObject
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetPressedImageObject") == 0) {
		stack->correctParams(0);
		if (!_imagePress) stack->pushNULL();
		else stack->pushNative(_imagePress, true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetFocusedImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "SetFocusedImage") == 0) {
		stack->correctParams(1);

		delete _imageFocus;
		_imageFocus = new CBSprite(_gameRef);
		const char *filename = stack->pop()->getString();
		if (!_imageFocus || DID_FAIL(_imageFocus->loadFile(filename))) {
			delete _imageFocus;
			_imageFocus = NULL;
			stack->pushBool(false);
		} else stack->pushBool(true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetFocusedImage
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetFocusedImage") == 0) {
		stack->correctParams(0);
		if (!_imageFocus || !_imageFocus->_filename) stack->pushNULL();
		else stack->pushString(_imageFocus->_filename);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetFocusedImageObject
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "GetFocusedImageObject") == 0) {
		stack->correctParams(0);
		if (!_imageFocus) stack->pushNULL();
		else stack->pushNative(_imageFocus, true);

		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Press
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "Press") == 0) {
		stack->correctParams(0);

		if (_visible && !_disable) {
			_oneTimePress = true;
			_oneTimePressTime = CBPlatform::getTime();
		}
		stack->pushNULL();

		return STATUS_OK;
	}


	else return CUIObject::scCallMethod(script, stack, thisStack, name);
}


//////////////////////////////////////////////////////////////////////////
CScValue *CUIButton::scGetProperty(const char *name) {
	_scValue->setNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if (strcmp(name, "Type") == 0) {
		_scValue->setString("button");
		return _scValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TextAlign
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "TextAlign") == 0) {
		_scValue->setInt(_align);
		return _scValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Focusable
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "Focusable") == 0) {
		_scValue->setBool(_canFocus);
		return _scValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// Pressed
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "Pressed") == 0) {
		_scValue->setBool(_stayPressed);
		return _scValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// PixelPerfect
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "PixelPerfect") == 0) {
		_scValue->setBool(_pixelPerfect);
		return _scValue;
	}

	else return CUIObject::scGetProperty(name);
}


//////////////////////////////////////////////////////////////////////////
bool CUIButton::scSetProperty(const char *name, CScValue *value) {
	//////////////////////////////////////////////////////////////////////////
	// TextAlign
	//////////////////////////////////////////////////////////////////////////
	if (strcmp(name, "TextAlign") == 0) {
		int i = value->getInt();
		if (i < 0 || i >= NUM_TEXT_ALIGN) i = 0;
		_align = (TTextAlign)i;
		return STATUS_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Focusable
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "Focusable") == 0) {
		_canFocus = value->getBool();
		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// Pressed
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "Pressed") == 0) {
		_stayPressed = value->getBool();
		return STATUS_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// PixelPerfect
	//////////////////////////////////////////////////////////////////////////
	else if (strcmp(name, "PixelPerfect") == 0) {
		_pixelPerfect = value->getBool();
		return STATUS_OK;
	}

	else return CUIObject::scSetProperty(name, value);
}


//////////////////////////////////////////////////////////////////////////
const char *CUIButton::scToString() {
	return "[button]";
}


//////////////////////////////////////////////////////////////////////////
bool CUIButton::persist(CBPersistMgr *persistMgr) {

	CUIObject::persist(persistMgr);

	persistMgr->transfer(TMEMBER_INT(_align));
	persistMgr->transfer(TMEMBER(_backDisable));
	persistMgr->transfer(TMEMBER(_backFocus));
	persistMgr->transfer(TMEMBER(_backHover));
	persistMgr->transfer(TMEMBER(_backPress));
	persistMgr->transfer(TMEMBER(_centerImage));
	persistMgr->transfer(TMEMBER(_fontDisable));
	persistMgr->transfer(TMEMBER(_fontFocus));
	persistMgr->transfer(TMEMBER(_fontHover));
	persistMgr->transfer(TMEMBER(_fontPress));
	persistMgr->transfer(TMEMBER(_hover));
	persistMgr->transfer(TMEMBER(_image));
	persistMgr->transfer(TMEMBER(_imageDisable));
	persistMgr->transfer(TMEMBER(_imageFocus));
	persistMgr->transfer(TMEMBER(_imageHover));
	persistMgr->transfer(TMEMBER(_imagePress));
	persistMgr->transfer(TMEMBER(_pixelPerfect));
	persistMgr->transfer(TMEMBER(_press));
	persistMgr->transfer(TMEMBER(_stayPressed));

	if (!persistMgr->_saving) {
		_oneTimePress = false;
		_oneTimePressTime = 0;
	}

	return STATUS_OK;
}

} // end of namespace WinterMute
