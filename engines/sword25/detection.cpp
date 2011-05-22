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

#include "base/plugins.h"
#include "engines/advancedDetector.h"

#include "sword25/sword25.h"
#include "sword25/kernel/persistenceservice.h"

namespace Sword25 {
uint32 Sword25Engine::getGameFlags() const { return _gameDescription->flags; }
}

static const PlainGameDescriptor Sword25Game[] = {
	{"sword25", "Broken Sword 2.5"},
	{0, 0}
};

namespace Sword25 {

static const ADGameDescription gameDescriptions[] = {
	// These two versions represent the default languages available in the data file
	{
		"sword25",
		"",
		AD_ENTRY1s("data.b25c", "f8b6e03ada2d2f6cf27fbc11ad1572e9", 654310588),
		Common::EN_ANY,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("lang_fr.b25c", "690caf157387e06d2c3d1ca53c43f428", 1006043),
		Common::FR_FRA,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("data.b25c", "f8b6e03ada2d2f6cf27fbc11ad1572e9", 654310588),
		Common::DE_DEU,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("lang_hr.b25c", "e881054d1f8ec1e527422fc521c25405", 1273217),
		Common::HU_HUN,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("lang_it.b25c", "f3325666da0515cc2b42062e953c0889", 996197),
		Common::IT_ITA,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("lang_pl.b25c", "49dc1a20f95391a808e475c49be2bac0", 1281799),
		Common::PL_POL,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("lang_pt.b25c", "1df701432f9e13dcefe1adeb890b9c69", 993812),
		Common::PT_BRA,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("lang_ru.b25c", "deb33dd2f90a71ff60181918a8ce5063", 1235378),
		Common::RU_RUS,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},
	{
		"sword25",
		"",
		AD_ENTRY1s("lang_es.b25c", "384c19072d83725f351bb9ecb4d3f02b", 987965),
		Common::ES_ESP,
		Common::kPlatformUnknown,
		ADGF_NO_FLAGS,
		Common::GUIO_NONE
	},

	// Extracted version
	{
		"sword25",
		"Extracted",
		{{"_includes.lua", 0, 0, -1},
		 {"boot.lua", 0, 0, -1},
		 {"kernel.lua", 0, 0, -1},
		 AD_LISTEND},
		Common::EN_ANY,
		Common::kPlatformUnknown,
		GF_EXTRACTED,
		Common::GUIO_NONE
	},
	AD_TABLE_END_MARKER
};

} // End of namespace Sword25

static const char *directoryGlobs[] = {
	"system", // Used by extracted dats
	0
};

static const ADParams detectionParams = {
	// Pointer to ADGameDescription or its superset structure
	(const byte *)Sword25::gameDescriptions,
	// Size of that superset structure
	sizeof(ADGameDescription),
	// Number of bytes to compute MD5 sum for
	5000,
	// List of all engine targets
	Sword25Game,
	// Structure for autoupgrading obsolete targets
	0,
	// Name of single gameid (optional)
	NULL,
	// List of files for file-based fallback detection (optional)
	0,
	// Flags
	0,
	// Additional GUI options (for every game}
	Common::GUIO_NOMIDI,
	// Maximum directory depth
	2,
	// List of directory globs
	directoryGlobs
};

class Sword25MetaEngine : public AdvancedMetaEngine {
public:
	Sword25MetaEngine() : AdvancedMetaEngine(detectionParams) {}

	virtual const char *getName() const {
		return "Sword25";
	}

	virtual const char *getOriginalCopyright() const {
		return "Broken Sword 2.5 (C) Malte Thiesen, Daniel Queteschiner and Michael Elsdorfer";
	}

	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual int getMaximumSaveSlot() const { return Sword25::PersistenceService::getSlotCount(); }
	virtual SaveStateList listSaves(const char *target) const;
};

bool Sword25MetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	if (desc) {
		*engine = new Sword25::Sword25Engine(syst, desc);
	}
	return desc != 0;
}

bool Sword25MetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves);
}

SaveStateList Sword25MetaEngine::listSaves(const char *target) const {
	Common::String pattern = target;
	pattern = pattern + ".???";
	SaveStateList saveList;

	Sword25::PersistenceService ps;
	Sword25::setGameTarget(target);

	ps.reloadSlots();

	for (uint i = 0; i < ps.getSlotCount(); ++i) {
		if (ps.isSlotOccupied(i)) {
			Common::String desc = ps.getSavegameDescription(i);
			saveList.push_back(SaveStateDescriptor(i, desc));
		}
	}

	return saveList;
}

#if PLUGIN_ENABLED_DYNAMIC(SWORD25)
	REGISTER_PLUGIN_DYNAMIC(SWORD25, PLUGIN_TYPE_ENGINE, Sword25MetaEngine);
#else
	REGISTER_PLUGIN_STATIC(SWORD25, PLUGIN_TYPE_ENGINE, Sword25MetaEngine);
#endif

