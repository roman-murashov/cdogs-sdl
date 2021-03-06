#define SDL_MAIN_HANDLED
#include <cbehave/cbehave.h>

#include <config_io.h>
#include <config_json.h>
#include <config_old.h>
#include <pic_manager.h>
#include <sounds.h>
#include <weapon.h>

// Stubs
Mix_Chunk *StrSound(const char *s)
{
	UNUSED(s);
	return NULL;
}
Pic *PicManagerGetFromOld(PicManager *pm, int idx)
{
	UNUSED(pm);
	UNUSED(idx);
	return NULL;
}
Pic *PicManagerGetPic(const PicManager *pm, const char *name)
{
	UNUSED(pm);
	UNUSED(name);
	return NULL;
}
const GunDescription *StrGunDescription(const char *s)
{
	UNUSED(s);
	return NULL;
}
const char *JoyName(const int deviceIndex)
{
	UNUSED(deviceIndex);
	return NULL;
}
PicManager gPicManager;


FEATURE(1, "Load default config")
	SCENARIO("Load a default config")
		GIVEN("two configs")
			Config config1, config2;

		WHEN("I load them both with defaults")
			// Note: default loaded before loading from file
			config1 = ConfigLoad(NULL);
			config2 = ConfigLoad(NULL);

		THEN("the two configs should have the same values")
			SHOULD_INT_EQUAL(
				ConfigGetBool(&config1, "Game.FriendlyFire"),
				ConfigGetBool(&config2, "Game.FriendlyFire"));
			SHOULD_INT_EQUAL(
				ConfigGetInt(&config1, "Graphics.Brightness"),
				ConfigGetInt(&config2, "Graphics.Brightness"));
	SCENARIO_END
FEATURE_END

FEATURE(2, "Save and load")
	SCENARIO("Save and load a JSON config file")
		GIVEN("a config file with some values, and I save the config to a JSON file")
			Config config1 = ConfigLoad(NULL);
			ConfigGet(&config1, "Game.FriendlyFire")->u.Bool.Value = true;
			ConfigGet(&config1, "Graphics.Brightness")->u.Int.Value = 5;
			ConfigSave(&config1, "tmp");

		WHEN("I load a second config from that file")
			Config config2 = ConfigLoad("tmp");

		THEN("the two configs should have the same values")
			SHOULD_INT_EQUAL(
				ConfigGetBool(&config1, "Game.FriendlyFire"),
				ConfigGetBool(&config2, "Game.FriendlyFire"));
			SHOULD_INT_EQUAL(
				ConfigGetInt(&config1, "Graphics.Brightness"),
				ConfigGetInt(&config2, "Graphics.Brightness"));
	SCENARIO_END
FEATURE_END

FEATURE(3, "Detect config version")
	SCENARIO("Detect JSON config version")
		GIVEN("a config file with some values, and I save the config to file in the JSON format")
			Config config1 = ConfigLoad(NULL);
			ConfigGet(&config1, "Game.FriendlyFire")->u.Bool.Value = true;
			ConfigGet(&config1, "Graphics.Brightness")->u.Int.Value = 5;
			ConfigSave(&config1, "tmp");

		WHEN("I detect the version, and load a second config from that file")
			FILE *file = fopen("tmp", "r");
			int version = ConfigGetVersion(file);
			fclose(file);
			Config config2 = ConfigLoad("tmp");

		THEN("the version should be " TOSTRING(CONFIG_VERSION))
			SHOULD_INT_EQUAL(version, CONFIG_VERSION);
		AND("the two configs should have the same values")
			SHOULD_INT_EQUAL(
				ConfigGetBool(&config1, "Game.FriendlyFire"),
				ConfigGetBool(&config2, "Game.FriendlyFire"));
			SHOULD_INT_EQUAL(
				ConfigGetInt(&config1, "Graphics.Brightness"),
				ConfigGetInt(&config2, "Graphics.Brightness"));
	SCENARIO_END
FEATURE_END

FEATURE(4, "Save config as latest format by default")
	SCENARIO("Save as JSON by default")
	{
		Config config;
		int version;
		FILE *file;
		GIVEN("a config file with some values, and I save the config to file")
			config = ConfigLoad(NULL);
			ConfigGet(&config, "Game.FriendlyFire")->u.Bool.Value = true;
			ConfigGet(&config, "Graphics.Brightness")->u.Int.Value = 5;
			ConfigSave(&config, "tmp");

		WHEN("I detect the version")
			file = fopen("tmp", "r");
			version = ConfigGetVersion(file);
			fclose(file);

		THEN("the version should be " TOSTRING(CONFIG_VERSION))
			SHOULD_INT_EQUAL(version, CONFIG_VERSION);
	}
	SCENARIO_END
FEATURE_END

int main(void)
{
	cbehave_feature features[] =
	{
		{feature_idx(1)},
		{feature_idx(2)},
		{feature_idx(3)},
		{feature_idx(4)}
	};

	return cbehave_runner("Config features are:", features);
}
