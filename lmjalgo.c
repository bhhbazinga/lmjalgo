/*************************************************************************
FileName: lmjalgo.c
Author: turbobhh
Mail: turbobhh@gmail.com
CreatedTime: Thu 08 Jun 2017 08:38:06 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include "mjalgo.h"

#if LUA_VERSION_NUM < 502
# ifndef luaL_newlib
#  define luaL_newlib(L, l) (lua_newtable(L), luaL_register(L, NULL, l))
# endif
# ifndef lua_setuservalue
#  define lua_setuservalue(L, n) lua_setfenv(L, n)
# endif
# ifndef lua_getuservalue
#  define lua_getuservalue(L, n) lua_getfenv(L, n)
# endif
#endif

#define LUA_BASE_NUM 11
#define LUA_BASE_EYE 1
#define NUM_LUA2C(num) ((num) - LUA_BASE_NUM)
#define NUM_C2LUA(num) ((num) + LUA_BASE_NUM)
#define EYE_LUA2C(eye) (eye + LUA_BASE_EYE)
#define EYE_C2LUA(eye) (eye - LUA_BASE_EYE)

static void get_pais(lua_State *L, int index, pais_t pais)
{
	luaL_checktype(L, index, LUA_TTABLE);
	int cnt;
	for (int i = 0; i < 4; ++i) {
		lua_rawgeti(L, index, i + 1);
		luaL_checktype(L, -1, LUA_TTABLE);
		for (int j = 0; j < 10; ++j) {
			lua_rawgeti(L, -1, j + 1);
			cnt = luaL_checkinteger(L, -1);
			pais[i][j] = cnt;
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
}

static int get_nums(lua_State*L, int index, nums_t nums)
{
	if (lua_isnoneornil(L, index)) return 0;

	int num, n;
	n = lua_rawlen(L, index);
	for (int i = 0; i < n; ++i) {
		lua_rawgeti(L, index, i + 1);
		num = luaL_checkinteger(L, -1);
		nums[i] = NUM_LUA2C(num);
	}
	return n;
}

static void push_pais(lua_State *L, pais_t pais)
{
	lua_newtable(L);
	for (int i = 0; i < 4; ++i) {
		lua_newtable(L);
		for (int j = 0; j < 10; ++j) {
			lua_pushinteger(L, NUM_C2LUA(pais[i][j]));
			lua_rawseti(L, -2, j + 1);
		}
		lua_rawseti(L, -2, i + 1);
	}
}

static void push_tingdata(lua_State* L, tingmap_t tmap)
{
	struct tingnode_s *tnode;

	lua_newtable(L);
	TINGMAP_FOREACH_BEGEIN(tmap, num, tnodes, n)
		lua_newtable(L);
		for (int i = 0; i < n; ++i) {
			tnode = tnodes[i];
			lua_newtable(L);
			lua_pushinteger(L, NUM_C2LUA(tnode->num));
			lua_rawseti(L, -2, 1);
			lua_pushinteger(L, tnode->mask);
			lua_rawseti(L, -2, 2);
			lua_rawseti(L, -2, i + 1);
		}
		lua_rawseti(L, -2, NUM_C2LUA(num));
	TINGMAP_FOREACH_END
}

static int lcheckhu_jh(lua_State *L)
{
	bool hu;
	int eyei, eyej;

	pais_t pais;
	get_pais(L, 1, pais);
	hu = checkhu_jh(pais, &eyei, &eyej);

	lua_pushboolean(L, hu);
	lua_pushinteger(L, EYE_C2LUA(eyei));
	lua_pushinteger(L, EYE_C2LUA(eyej));

	if (hu) {
		lua_pushinteger(L, EYE_C2LUA(eyei));
		lua_pushinteger(L, EYE_C2LUA(eyej));
	}
	return hu ? 3 : 1;
}

static int lg_checkhu_jh(lua_State *L)
{
	pais_t pais, gpais;
	int gnums[G_MAX_COUNT], n, eyei, eyej;
	bool hu;

	get_pais(L, 1, pais);
	n = get_nums(L, 2, gnums);
	hu = g_checkhu_jh(pais, gnums, n, gpais, &eyei, &eyej);

	lua_pushboolean(L, hu);
	if (hu) {
		push_pais(L, gpais);
		lua_pushinteger(L, EYE_C2LUA(eyei));
		lua_pushinteger(L, EYE_C2LUA(eyej));
	}
	return hu ? 4 : 1;
}

static int lg_gettingdata(lua_State *L)
{
	tingmap_t tmap;
	pais_t pais;
	nums_t gnums, lnums, nums;
	humask_t mask;
	int gn, ln, n;
	bool hu;

	get_pais(L, 1, pais);
	gn   = get_nums(L, 2, gnums);
	ln   = get_nums(L, 3, lnums);
	n    = get_nums(L, 4, nums);
	mask = luaL_checkinteger(L, 5);

	TINGMAP_INIT(tmap);
	hu = g_gettingdata(pais,
			    gnums, gn,
			    lnums, ln,
			    nums,  n,
			    mask,
			    tmap);
	hu ? push_tingdata(L, tmap) : lua_pushnil(L);
	TINGMAP_FREE(tmap);
	return 1;
}

static int lg_gethumask(lua_State *L)
{
	pais_t pais;
	nums_t gnums, nums;
	humask_t mask;
	humask_t res_mask;
	int gn, n;

	get_pais(L, 1, pais);
	gn   = get_nums(L, 2, gnums);
	n    = get_nums(L, 3, nums);
	mask = luaL_checkinteger(L, 4);

	res_mask = g_gethumask(pais,
			       gnums, gn,
			       nums,  n,
			       mask);
	lua_pushinteger(L, res_mask);
	return 1;
}

#define REG_MASK(L, mask) (lua_pushinteger(L, mask), lua_setfield(L, -2, #mask))
int luaopen_lmjalgo(lua_State *L)
{
	luaL_Reg lfuncs[] = {
		{"checkhu_jh", lcheckhu_jh},
		{"g_checkhu_jh", lg_checkhu_jh},
		{"g_gettingdata", lg_gettingdata},
		{"g_gethumask", lg_gethumask},
		{NULL, NULL},
	};
	luaL_newlib(L, lfuncs);
	REG_MASK(L, JH);
	REG_MASK(L, PPH);
	REG_MASK(L, HYS);
	REG_MASK(L, QXD);
	REG_MASK(L, LQD);
	REG_MASK(L, QYS);
	REG_MASK(L, HYJ);
	REG_MASK(L, XSY);
	REG_MASK(L, XSX);
	REG_MASK(L, ZYS);
	REG_MASK(L, QYJ);
	REG_MASK(L, SSY);
	REG_MASK(L, DSY);
	REG_MASK(L, DSX);
	REG_MASK(L, ALL);
	REG_MASK(L, NONE);
	return 1;
}
