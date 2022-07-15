#include "api.h"
#include "graphics/graphics.h"
#include "data/image.h"
#include "util.h"
#include <lua.h>
#include <lauxlib.h>

static int l_lovrReadbackIsComplete(lua_State* L) {
  Readback* readback = luax_checktype(L, 1, Readback);
  bool complete = lovrReadbackIsComplete(readback);
  lua_pushboolean(L, complete);
  return 1;
}

static int l_lovrReadbackWait(lua_State* L) {
  Readback* readback = luax_checktype(L, 1, Readback);
  bool waited = lovrReadbackWait(readback);
  lua_pushboolean(L, waited);
  return 1;
}

static int l_lovrReadbackGetData(lua_State* L) {
  Readback* readback = luax_checktype(L, 1, Readback);
  const ReadbackInfo* info = lovrReadbackGetInfo(readback);
  void* data = lovrReadbackGetData(readback);
  uint32_t* u32 = data;
  switch (info->type) {
    case READBACK_BUFFER:
      // TODO
      return 0;
    case READBACK_TEXTURE:
      lua_pushnil(L);
      return 1;
    case READBACK_TALLY: {
      int count = (int) info->tally.count;

      if (lovrTallyGetInfo(info->tally.object)->type == TALLY_STAGE) {
        count *= 6; // The number of pipeline statistics that are tracked
      }

      lua_createtable(L, count, 0);
      for (int i = 0; i < count; i++) {
        lua_pushinteger(L, u32[i]);
        lua_rawseti(L, -2, i + 1);
      }
      return 1;
    }
  }
}

static int l_lovrReadbackGetBlob(lua_State* L) {
  //Readback* readback = luax_checktype(L, 1, Readback);
  //void* data = lovrReadbackGetData(readback);
  lua_pushnil(L);
  return 1;
}

static int l_lovrReadbackGetImage(lua_State* L) {
  Readback* readback = luax_checktype(L, 1, Readback);
  Image* image = lovrReadbackGetImage(readback);
  luax_pushtype(L, Image, image);
  return 1;
}

const luaL_Reg lovrReadback[] = {
  { "isComplete", l_lovrReadbackIsComplete },
  { "wait", l_lovrReadbackWait },
  { "getData", l_lovrReadbackGetData },
  { "getBlob", l_lovrReadbackGetBlob },
  { "getImage", l_lovrReadbackGetImage },
  { NULL, NULL }
};
