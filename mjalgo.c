/*************************************************************************
FileName: mjalgo.c
Author: turbobhh
Mail: turbobhh@gmail.com
CreatedTime: Thu 08 Jun 2017 08:37:54 PM CST
************************************************************************/
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include "mjalgo.h"

#define CHECK_COND(cond) do {if (!(cond)) return false;} while(0)

static bool dfs_shunke(kpais_t kpais, int k)
{
	if (0 == kpais[9]) return true;

	int j;
	for (int i = 0; i < 9; ++i) {
		if (0 != kpais[i]) {
			j = i;
			break;
		}
	}

	bool success = false;
	if (kpais[j] > 2) {
		kpais[j] -= 3;
		kpais[9] -= 3;
		success = dfs_shunke(kpais, k);
		kpais[j] += 3;
		kpais[9] += 3;
		return success;
	}

	if (KIND_ZI != k && j < 7 && kpais[j + 1] > 0 && kpais[j + 2] > 0) {
		--kpais[j];
		--kpais[j + 1];
		--kpais[j + 2];
		kpais[9] -= 3;
		success = dfs_shunke(kpais, k);
		++kpais[j];
		++kpais[j + 1];
		++kpais[j + 2];
		kpais[9] += 3;
		return success;
	}

	return success;
}

static bool g_dfs_shunke(kpais_t kpais, int k, int *p_leftcnt, pais_t gpais)
{
	if (kpais[9] == 0) return true;

	int j;
	for (int i = 0; i < 9; ++i) {
		if (0 != kpais[i]) {
			j = i;
			break;
		}
	}

	bool success = false;
	if (kpais[j] > 2) {
		kpais[j] -= 3;
		kpais[9] -= 3;
		success = g_dfs_shunke(kpais,
				       k,
				       p_leftcnt,
				       gpais);
		kpais[j] += 3;
		kpais[9] += 3;
		return success;
	}

	int needcnt = 3 - kpais[j];
	int jcnt;
	if (needcnt <= *p_leftcnt) {
		jcnt = kpais[j];
		kpais[j] = 0;
		kpais[9] -= jcnt;
		*p_leftcnt -= needcnt;
		success = g_dfs_shunke(kpais,
				       k,
				       p_leftcnt,
				       gpais);
		kpais[j] = jcnt;
		kpais[9] += jcnt;
		if (success) {
			gpais[k][j] += needcnt;
			gpais[k][9] += needcnt;
			return success;
		} else *p_leftcnt += needcnt;
	}

	CHECK_COND(k != KIND_ZI);

	if (j < 7 && kpais[j + 1] > 0 && kpais[j + 2] > 0) {
		--kpais[j];
		--kpais[j + 1];
		--kpais[j + 2];
		kpais[9] -= 3;
		success = g_dfs_shunke(kpais,
				       k,
				       p_leftcnt,
				       gpais);
		++kpais[j];
		++kpais[j + 1];
		++kpais[j + 2];
		kpais[9] += 3;
		return success;
	}

	int j1subcnt, j2subcnt;
	if (7 == j) {
		j1subcnt = kpais[j - 1] > 0;
		j2subcnt = kpais[j + 1] > 0;
		needcnt = 2 - j1subcnt - j2subcnt;
		if (needcnt <= *p_leftcnt) {
			--kpais[j];
			kpais[j - 1] -= j1subcnt;
			kpais[j + 1] -= j2subcnt;
			kpais[9] -= (1 + j1subcnt + j2subcnt);
			*p_leftcnt -= needcnt;
			success = g_dfs_shunke(kpais,
					       k,
					       p_leftcnt,
					       gpais);
			++kpais[j];
			kpais[j - 1] += j1subcnt;
			kpais[j + 1] += j2subcnt;
			kpais[9] += (1 + j1subcnt + j2subcnt);
		}
		if (success) {
			gpais[k][j - 1] += j1subcnt <= 0;
			gpais[k][j + 1] += j2subcnt <= 0;
			gpais[k][9] += ((j1subcnt <= 0) + (j2subcnt <= 0));
		}
		return success;
	}

	if (8 == j) {
		j1subcnt = kpais[j - 1] > 0;
		j2subcnt = kpais[j - 2] > 0;
		needcnt = 2 - j1subcnt - j2subcnt;
		if (needcnt <= *p_leftcnt) {
			--kpais[j];
			kpais[j - 1] -= j1subcnt;
			kpais[j - 2] -= j2subcnt;
			kpais[9] -= (1 + j1subcnt + j2subcnt);
			*p_leftcnt -= needcnt;
			success = g_dfs_shunke(kpais,
					       k,
					       p_leftcnt,
					       gpais);
			++kpais[j];
			kpais[j - 1] += j1subcnt;
			kpais[j - 2] += j2subcnt;
			kpais[9] += (1 + j1subcnt + j2subcnt);
		}
		if (success) {
			gpais[k][j - 1] += j1subcnt <= 0;
			gpais[k][j - 2] += j2subcnt <= 0;
			gpais[k][9] += ((j1subcnt <= 0) + (j2subcnt <= 0));
		}
		return success;
	}

	// j < 7
	j1subcnt = kpais[j + 1] > 0;
	j2subcnt = kpais[j + 2] > 0;
	needcnt = 2 - j1subcnt - j2subcnt;
	if (needcnt <= *p_leftcnt) {
		--kpais[j];
		kpais[j + 1] -= j1subcnt;
		kpais[j + 2] -= j2subcnt;
		kpais[9] -= (1 + j1subcnt + j2subcnt);
		*p_leftcnt -= needcnt;
		success = g_dfs_shunke(kpais,
				       k,
				       p_leftcnt,
				       gpais);
		++kpais[j];
		kpais[j + 1] += j1subcnt;
		kpais[j + 2] += j2subcnt;
		kpais[9] += (1 + j1subcnt + j2subcnt);
	}
	if (success) {
		gpais[k][j + 1] += j1subcnt <= 0;
		gpais[k][j + 1] += j2subcnt <= 0;
		gpais[k][9] += ((j1subcnt <= 0) + (j2subcnt <= 0));
	}
	return success;
}

inline static int g_filter(pais_t pais, nums_t gnums, int n, nums_t m_gnums)
{
	int gcnt = 0,curcnt;
	int gnum, gi, gj;
	int idx = 0;
	for (int i = 0; i < n; ++i) {
		gnum = gnums[i];
		gi = NUM2I(gnum);
		gj = NUM2J(gnum);
		curcnt = pais[gi][gj];
		if (curcnt <= 0) continue;

		gcnt += pais[gi][gj];
		m_gnums[idx * 2] = gnum;
		m_gnums[idx * 2 + 1] = curcnt;
		PAIS_DELETE(pais, gi, gj, curcnt);
		++idx;
	}

	return gcnt;
}

inline static void g_reset(pais_t pais, nums_t m_gnums, int n)
{
	int gnum, gcnt, gi, gj;
	for (int i = 0; i < n; ++i) {
		gnum = m_gnums[i * 2];
		if (gnum == -1) break;
		gcnt = m_gnums[i * 2 + 1];
		gi = NUM2I(gnum);
		gj = NUM2J(gnum);
		PAIS_INSERT(pais, gi, gj, gcnt);
	}
}

inline static bool g_check3n(pais_t pais, int gcnt)
{
	int needcnt = 0, remainder;
	for (int i = 0; i < 4; ++i) {
		remainder = pais[i][9] % 3;
		needcnt += remainder == 0 ? remainder : (3 - remainder);
		CHECK_COND(needcnt <= gcnt);
	}
	return true;
}

inline static bool g_contains(nums_t gnums, int n, int num)
{
	for (int i = 0; i < n; ++i) if (gnums[i] == num) return true;
	return false;
}

/* *
 * pais : 手牌
 * nums : 碰或杠的牌
 * n    : nums数组大小
 * jh   : 是否鸡胡
 * eyei : 将牌花色
 * eyej : 将牌数值
 * */

inline static bool checkhu_jh(pais_t pais, int *eyei, int *eyej)
{
	int eye_pos, remainder;
	bool eye_existed = false;

	for (int i = 0; i < 4; ++i) {
		remainder = pais[i][9] % 3;
		CHECK_COND(remainder != 1);
		if (remainder == 2) {
			CHECK_COND(!eye_existed);
			eye_pos = i;
			eye_existed = true;
		}
	}

	CHECK_COND(eye_existed);
	eye_existed = false;
	for (int i = 0; i < 9; ++i) {
		if (pais[eye_pos][i] == 2) {
			*eyei = eye_pos;
			*eyej = i;
			eye_existed = true;
		}
	}
	CHECK_COND(eye_existed);

	for (int k = 0; k < 4; ++k) if (k != eye_pos) CHECK_COND(dfs_shunke(pais[k], k));

	bool success = false;
	int j = *eyej;
	if (pais[eye_pos][j] > 1) {
		pais[eye_pos][j] -= 2;
		pais[eye_pos][9] -= 2;
		success = dfs_shunke(pais[eye_pos], eye_pos);
		pais[eye_pos][j] += 2;
		pais[eye_pos][9] += 2;
		if (success) return success;
	}
	return success;
}

inline static bool checkhu_pph(pais_t pais, bool jh, int n)
{
	CHECK_COND(jh);

	int cnt = n;
	PAIS_FOREACH(i, j) cnt += pais[i][j] >= 3;
	return cnt == 4;
}

inline static bool checkhu_hys(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	bool zi_exist = pais[KIND_ZI][9] > 0;
	int kmap[4] = {0, 0, 0, 0}, k;
	for (int i = 0; i < 3; ++i) kmap[i] = pais[i][9] > 0;
	for (int i = 0; i < n; ++i) {
		k = NUM2I(nums[i]);
		kmap[k] = 1;
		zi_exist = zi_exist || k == KIND_ZI;
	}
	return zi_exist && kmap[0] + kmap[1] + kmap[2] + kmap[3] < 3;
}

inline static bool checkhu_qxd(pais_t pais)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	PAIS_FOREACH(i, j) CHECK_COND(pais[i][j] == 0 || pais[i][j] == 2);
	return true;
}

inline static bool checkhu_lqd(pais_t pais, bool qxd)
{
	CHECK_COND(qxd && 14 == PAIS_COUNT(pais));

	PAIS_FOREACH(i, j) if (pais[i][j] == 4) return true;
	return false;
}

inline static bool checkhu_qys(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	int kmap[4] = {0, 0, 0, 0}, k;
	for (int i = 0; i < 4; ++i) kmap[i] = pais[i][9] > 0;
	for (int i = 0; i < n; ++i) {
		k = NUM2I(nums[i]);
		kmap[k] = 1;
	}
	return kmap[0] + kmap[1] + kmap[2] + kmap[3] == 1;
}

inline static bool checkhu_hyj(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	bool zi_exist = pais[KIND_ZI][9] > 0;
	int ni, nj, num;
	for (int i = 1; i < 3; ++i)
		for(int j = 1; j < 8; ++j) CHECK_COND(pais[i][j] <= 0);
	for (int i = 0; i < n; ++i) {
		num = nums[i];
		ni = NUM2I(num);
		nj = NUM2J(num);
		CHECK_COND(KIND_ZI == ni || 0 == ni || 8 == nj);
		zi_exist = zi_exist || KIND_ZI == ni;
	}
	return zi_exist;
}

inline static bool checkhu_xsy(pais_t pais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh);

	int kecnt = (pais[3][4] >= 3) +
		    (pais[3][5] >= 3) +
		    (pais[3][6] >= 3);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 33 && nums[i] < NUM_MAX);

	return kecnt > 1 && KIND_ZI == eyei && eyej > 3;
}

inline static bool checkhu_xsx(pais_t pais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh);

	int kecnt = (pais[KIND_ZI][0] >= 3) +
		    (pais[KIND_ZI][1] >= 3) +
		    (pais[KIND_ZI][2] >= 3) +
		    (pais[KIND_ZI][3] >= 3);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34);
	return kecnt >= 3 && KIND_ZI == eyei && eyej < 4;
}

inline static bool checkhu_zys(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	for (int i = 0; i < KIND_ZI; ++i)
		for(int j = 0; j < 9; ++j) CHECK_COND(pais[i][j] <= 0);
	for (int i = 0; i < n; ++i) CHECK_COND(KIND_ZI == NUM2I(nums[i]));
	return true;
}

inline static bool checkhu_qyj(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(pais[KIND_ZI][9] <= 0);

	for (int i = 0; i < KIND_ZI; ++i)
		for (int j = 1; j < 8; ++j) CHECK_COND(pais[i][j] <= 0);

	int k, j;
	for (int i = 0; i < n; ++i) {
		k = NUM2I(nums[i]);
		j = NUM2J(nums[i]);
		CHECK_COND(KIND_ZI != k && (0 == j || 8 == j));
	}
	return true;
}

inline static bool checkhu_ssy(pais_t pais)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	return pais[0][0] == 1 && pais[1][0] == 1 && pais[2][0] == 1 && pais[3][0] == 1 &&
	       pais[0][8] == 1 && pais[1][8] == 1 && pais[2][8] == 1 && pais[3][8] == 1 &&
	       pais[3][0] >= 1 && pais[3][1] >= 1 && pais[3][2] >= 1 && pais[3][3] >= 1 && pais[3][4] >= 1 && pais[3][5] >= 1 && pais[3][6] >= 1;
}

inline static bool checkhu_dsy(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	int kecnt = (pais[KIND_ZI][4] >= 3) +
		    (pais[KIND_ZI][5] >= 3) +
		    (pais[KIND_ZI][6] >= 3);
	for (int i = 0; i < n; ++i) kecnt += (nums[i] < NUM_MAX && nums[i] > 33);
	return kecnt;
}

inline static bool checkhu_dsx(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	int kecnt = (pais[KIND_ZI][0] >= 3) +
		    (pais[KIND_ZI][1] >= 3) +
		    (pais[KIND_ZI][2] >= 3) +
		    (pais[KIND_ZI][3] >= 3);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34);
	return kecnt == 4;
}

/* *
 * pais  : 手牌
 * gpais : 鬼牌变换以后的胡牌手牌
 * nums  : 碰或杠的牌
 * n     : nums数组大小
 * gnums : 鬼牌数组
 * gn    : gnums数组大小
 * jh    : 是否鸡胡
 * hu    : 鸡胡或龙七对或七小对
 * eyei  : 将牌花色
 * eyej  : 将牌数值
 * */


inline static bool g_checkhu_jh(pais_t pais, nums_t gnums, int n, pais_t gpais, int *eyei, int *eyej)
{
	int m_gnums[G_MAX_COUNT * 2] = {-1, -1, -1, -1}; // num|cnt|num|cnt
	int gcnt = g_filter(pais, gnums, n, m_gnums);

	if (gcnt <= 0 && checkhu_jh(pais, eyei, eyej)) {
		PAIS_COPY(gpais, pais);
		return true;
	}

	*eyei = *eyej = -1;
	PAIS_COPY(gpais, pais);
	bool success = false;
	int leftcnt = 0;

	PAIS_FOREACH(i, j) {
		if (pais[i][j] > 1) {
			pais[i][j] -= 2;
			pais[i][9] -= 2;
			if (!g_check3n(pais, gcnt)) goto _finish1;
			leftcnt = gcnt;
			for(int k = 0; k < 4; ++k) {
				success = g_dfs_shunke(pais[k],
						       k,
						       &leftcnt,
						       gpais);
				if (!success) break;
			}
_finish1:
			pais[i][j] += 2;
			pais[i][9] += 2;
			if (success) {
				*eyei = i;
				*eyej = j;
				goto _finish3;
			}
		}
	}

	PAIS_FOREACH(i, j) {
		if (pais[i][j] > 0) {
			--pais[i][j];
			--pais[i][9];
			if (!g_check3n(pais, gcnt)) goto _finish2;
			leftcnt = gcnt - 1;
			for(int k = 0; k < 4; ++k) {
				success = g_dfs_shunke(pais[k],
						       k,
						       &leftcnt,
						       gpais);
				if (!success) break;
			}
_finish2:
			++pais[i][j];
			++pais[i][9];
			if (success) {
				*eyei = i;
				*eyej = j;
				goto _finish3;
			}
		}
	}

	if (!g_check3n(pais, gcnt)) goto _finish3;
	leftcnt = gcnt - 2;
	for (int k = 0; k < 4; ++k) {
		success = g_dfs_shunke(pais[k],
				       k,
				       &leftcnt,
				       gpais);
		if (!success) break;
	}
_finish3:
	g_reset(pais, m_gnums, n);
	if (!success && gcnt > 3) {
		gpais[0][0] = *eyei = *eyej = - 1;
		success = true;
	}
	return success;
}

inline static bool g_checkhu_pph(pais_t gpais, bool jh, int n)
{
	CHECK_COND(-1 != gpais[0][0]);
	return checkhu_pph(gpais, jh, n);
}

inline static bool g_checkhu_hys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh, int eyei, bool qdh)
{
	CHECK_COND((jh && -1 != gpais[0][0]) || qdh);

	bool zi_exist = gpais[KIND_ZI][9] > 0 || (jh && (-1 == eyei));
	bool gconn;
	int kmap[4] = {0, 0, 0, 0}, k, num;

	for (int i = 0; i < 3; ++i) kmap[i] = gpais[i][9] > 0;

	for (int i = 0; i < n; ++i) {
		num = nums[i];
		k = NUM2I(num);
		gconn = g_contains(gnums, gn, num);
		kmap[k] = !gconn;
		zi_exist = zi_exist || k == KIND_ZI || gconn;
	}
	return zi_exist && kmap[0] + kmap[1] + kmap[2] + kmap[3] < 3;
}

inline static bool g_checkhu_qxd(pais_t pais, nums_t gnums, int gn, pais_t gpais)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	int m_gnums[G_MAX_COUNT * 2] = {-1, -1, -1, -1}; // num|cnt|num|cnt
	int gcnt = g_filter(pais, gnums, gn, m_gnums);
	if (gcnt <= 0) return checkhu_qxd(pais);

	bool success = false;
	PAIS_COPY(gpais, pais);
	PAIS_FOREACH(i, j) {
		if(pais[i][j] == 1 || pais[i][j] == 3) {
			--gcnt;
			++gpais[i][j];
		}
		success = gcnt >= 0;
		if (!success) goto _failed;
	}

	g_reset(pais, m_gnums, gn);
	return gcnt % 2 == 0;

_failed:
	g_reset(pais, m_gnums, gn);
	return false;
}

inline static bool g_checkhu_qys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh, bool qdh)
{
	CHECK_COND((jh && -1 != gpais[0][0]) || qdh);

	int kmap[4] = {0, 0, 0, 0}, k, num;
	for (int i = 0; i < 4; ++i) kmap[i] = gpais[i][9] > 0;

	for (int i = 0; i < n; ++i) {
		num = nums[i];
		k = NUM2I(num);
		kmap[k] = !g_contains(gnums, gn, num);
	}

	return kmap[0] + kmap[1] + kmap[2] + kmap[3] == 1;
}

inline static bool g_checkhu_lqd(pais_t pais, nums_t gnums, int gn, pais_t gpais)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	int m_gnums[G_MAX_COUNT * 2] = {-1, -1, -1, -1}; // num|cnt|num|cnt
	int gcnt = g_filter(pais, gnums, gn, m_gnums);
	if (gcnt <= 0) return checkhu_lqd(pais, checkhu_qxd(pais));

	bool exist4 = false;
	PAIS_COPY(gpais, pais);
	PAIS_FOREACH(i, j) {
		switch(pais[i][j]) {
			case 1 : --gcnt; ++gpais[i][j]; break;
			case 3 : --gcnt; ++gpais[i][j];
			case 4 : exist4 = true;
		}
		if (gcnt < 0) goto _failed;
	}

	g_reset(pais, m_gnums, gn);
	return gcnt % 2 == 0 && exist4;

_failed:
	g_reset(pais, m_gnums, gn);
	return false;
}

inline static bool g_checkhu_hyj(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh, int eyei)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	bool zi_exist = gpais[KIND_ZI][9] > 0 || -1 == eyei;
	for (int i = 0; i < KIND_ZI; ++i)
		for (int j = 1; j < 8; ++j) CHECK_COND(gpais[i][j] <= 0);

	int k, j, num;
	bool gconn;
	for (int i = 0; i < n; ++i) {
		num = nums[i];
		k = NUM2I(num);
		j = NUM2J(num);
		gconn = g_contains(gnums, gn, nums[i]);
		CHECK_COND(gconn || KIND_ZI == k || 0 == j || 8 == j);
		zi_exist = zi_exist || KIND_ZI == k || gconn;
	}
	return zi_exist;
}

inline static bool g_checkhu_xsy(pais_t gpais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	int kecnt = (gpais[KIND_ZI][4] >= 3) +
		    (gpais[KIND_ZI][5] >= 3) +
		    (gpais[KIND_ZI][6] >= 3);

	for (int i = 0; i < n; ++i) kecnt += nums[i] > 33;
	return kecnt > 1 && (-1 == eyei || (KIND_ZI == eyei && eyej > 3));
}

inline static bool g_checkhu_xsx(pais_t gpais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	int kecnt = (gpais[KIND_ZI][0] >= 3) +
		    (gpais[KIND_ZI][1] >= 3) +
		    (gpais[KIND_ZI][2] >= 3) +
		    (gpais[KIND_ZI][3] >= 3);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34);
	return kecnt >= 3 && (-1 == eyei || (KIND_ZI == eyei && eyej < 4));
}

inline static bool g_checkhu_zys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	for (int i = 0; i < KIND_ZI; ++i)
		for (int j = 0; j < 9; ++j) CHECK_COND(gpais[i][j] <= 0);
	for (int i = 0; i < n; ++i) CHECK_COND(KIND_ZI == NUM2I(nums[i]) || g_contains(gnums, gn, nums[i]));
	return true;
}

inline static bool g_checkhu_qyj(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(gpais[KIND_ZI][9] <= 0);
	for (int i = 0; i < KIND_ZI; ++i)
		for (int j = 1; j < 8; ++j) CHECK_COND(gpais[i][j] <= 0);

	int k, j;
	for (int i = 0; i < n; ++i) {
		k = NUM2I(nums[i]);
		j = NUM2J(nums[i]);
		CHECK_COND((KIND_ZI != k && (0 == j || 8 == j)) || g_contains(gnums, gn, nums[i]));
	}

	return true;
}

inline static bool g_checkhu_ssy(pais_t pais, nums_t gnums, int gn)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	int m_gnums[G_MAX_COUNT * 2] = {-1, -1, -1, -1}; // num|cnt|num|cnt
	int gcnt = g_filter(pais, gnums, gn, m_gnums), needcnt = 0;

	PAIS_FOREACH(i, j) if (KIND_ZI != i && 0 != j && 8 != j && pais[i][j] > 0) goto _failed;

	for (int i = 0; i < KIND_ZI; ++i) {
		if (pais[i][0] > 1 || pais[i][8] > 1) goto _failed;
		needcnt += ((pais[i][0] == 0) + (pais[i][8] == 0));
		if (needcnt > gcnt) goto _failed;
	}
	for (int i = 0; i < 7; ++i) {
		if (pais[KIND_ZI][i] > 2) goto _failed;
		needcnt += (pais[KIND_ZI][i] == 0);
		if (needcnt > gcnt) goto _failed;
	}

	g_reset(pais, m_gnums, gn);
	return true;

_failed:
	g_reset(pais, m_gnums, gn);
	return false;
}

inline static bool g_checkhu_dsy(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(jh && -1 != gpais[0][0]);
	int kecnt = (gpais[3][4] >= 3) +
		    (gpais[3][5] >= 3) +
		    (gpais[3][6] >= 3);
	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 33 || g_contains(gnums, gn, nums[i]));
	return kecnt >= 3;
}

inline static bool g_checkhu_dsx(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	int kecnt = (gpais[3][0] >= 3) +
		    (gpais[3][1] >= 3) +
		    (gpais[3][2] >= 3) +
		    (gpais[3][3] >= 3);
	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34) || g_contains(gnums, gn, nums[i]);

	return kecnt > 3;
}

inline static int getscore(humask_t mask)
{
	int score = 0;
	score += (mask & JH)  ? 2 : 0;
	score += (mask & PPH) ? 8 : 0;
	score += (mask & HYS) ? 8 : 0;
	score += (mask & QXD) ? 16 : 0;
	score += (mask & QYS) ? 16 : 0;
	score += (mask & LQD) ? 32 : 0;
	score += (mask & HYJ) ? 32 : 0;
	score += (mask & XSY) ? 48 : 0;
	score += (mask & XSX) ? 48 : 0;
	score += (mask & ZYS) ? 64 : 0;
	score += (mask & QYJ) ? 64 : 0;
	score += (mask & SSY) ? 88 : 0;
	score += (mask & DSY) ? 88 : 0;
	score += (mask & DSX) ? 88 : 0;
	return score;
}

inline static bool insert_tingnode(QUEUE* h, int num, humask_t mask)
{
	struct tingnode_s *tnode = malloc(sizeof(struct tingnode_s));
	if (NULL == tnode) goto _nonmem;
	tnode->num = num;
	tnode->score = getscore(mask);
	tnode->mask = mask;
	QUEUE_INIT(&tnode->node);
	QUEUE_INSERT_TAIL(h, &tnode->node);
_nonmem:
	return true;
}

bool g_gettingdata(pais_t pais,
		nums_t gnums, int gn,
		nums_t lnums, int ln,
		nums_t nums , int n,
		humask_t mask,
		tingmap_t tmap)
{
	int num, lnum, li, lj;
	bool hu = false;
	humask_t hmask;

	PAIS_FOREACH(i, j) {
		if (pais[i][j] <= 0) continue;

		PAIS_DELETE(pais, i, j, 1);
		num = IJ2NUM(i, j);
		for (int l = 0; l < ln; ++l) {
			lnum = lnums[l];
			li = NUM2I(lnum);
			lj = NUM2J(lnum);
			if (lj == 9) continue;

			PAIS_INSERT(pais, li, lj, 1);
			hmask = g_gethumask(pais,
					gnums, gn,
					nums , n,
					mask);
			hmask && insert_tingnode(&tmap[num], lnum, hmask);
			hu = hu || hmask;
			PAIS_DELETE(pais, li, lj, 1);
		}
		PAIS_INSERT(pais, i, j, 1);
	}
	return hu;
}

humask_t g_gethumask(pais_t pais,
		nums_t gnums, int gn,
		nums_t nums , int n,
		humask_t mask)
{
	pais_t gpais;
	int eyei, eyej;
	int max_score = 0;
	humask_t mjh = 0, mpph = 0, mhys = 0, mqxd = 0, mlqd = 0, mqys = 0, mhyj = 0,
		 mxsy = 0, mxsx = 0, mzys = 0, mqyj = 0, mssy = 0, mdsy = 0, mdsx = 0,
		 tmp1, tmp2,
		 max_mask = NONE;

	/*hys, qys可以和常规胡或七对胡叠加*/
	mqxd = (QXD & mask) && g_checkhu_qxd(pais, gnums, gn, gpais) ? QXD : NONE;
	mlqd = (LQD & mask) && g_checkhu_lqd(pais, gnums, gn, gpais) ? LQD : NONE;
	mhys = (HYS & mask) && g_checkhu_hys(gpais, nums, n, gnums, gn, mjh, eyei, mqxd || mlqd) ? HYS : NONE;
	mqys = (QYS & mask) && g_checkhu_qys(gpais, nums, n, gnums, gn, mjh, mqxd || mlqd) ? QYS : NONE;
	tmp1 = mlqd | mhys | mqys;
	tmp2 = mqxd | mhys | mqys;
	if (tmp1 || tmp2) return getscore(tmp1) > getscore(tmp2) ? tmp1 : tmp2;

	mjh = g_checkhu_jh(pais, gnums, gn, gpais, &eyei, &eyej) ? JH : NONE;
	mpph = (PPH & mask) && g_checkhu_pph(gpais, mjh, n) ? PPH : NONE;
	mhyj = (HYJ & mask) && g_checkhu_hyj(gpais, nums, n, gnums, gn, mjh, eyei) ? HYJ : NONE;
	mxsy = (XSY & mask) && g_checkhu_xsy(gpais, nums, n, mjh, eyei, eyej) ? XSY : NONE;
	mxsx = (XSX & mask) && g_checkhu_xsx(gpais, nums, n, mjh, eyei, eyej) ? XSX : NONE;
	mzys = (ZYS & mask) && g_checkhu_zys(gpais, nums, n, gnums, gn, mjh) ? ZYS : NONE;
	mqyj = (QYJ & mask) && g_checkhu_qyj(gpais, nums, n, gnums, gn, mjh) ? QYJ : NONE;
	mssy = (SSY & mask) && g_checkhu_ssy(pais, gnums, gn) ? SSY : NONE;
	mdsy = (DSY & mask) && g_checkhu_dsy(gpais, nums, n, gnums, gn, mjh) ? DSY : NONE;
	mdsx = (DSX & mask) && g_checkhu_dsx(gpais, nums, n, gnums, gn, mjh) ? DSX : NONE;

	if ((tmp1 = mdsx | mhys | mzys) && (tmp2 = getscore(tmp1)) > max_score) {max_mask = tmp1; max_score = tmp2;}
	if ((tmp1 = mdsy | mhys | mzys) && (tmp2 = getscore(tmp1)) > max_score) {max_mask = tmp1; max_score = tmp2;}
	if (mssy && (tmp2 = getscore(mssy)) > max_score) {max_mask = mssy; max_score = tmp2;}
	if (mqyj && (tmp2 = getscore(mqyj)) > max_score) {max_mask = mqyj; max_score = tmp2;}
	if ((tmp1 = mzys | mxsy | mxsx | mdsy | mdsx) && (tmp2 = getscore(tmp1)) > max_score) {max_mask = tmp1; max_score = tmp2;}
	if ((tmp1 = mxsx | mhys | mzys) && (tmp2 = getscore(tmp1)) > max_score) {max_mask = tmp1; max_score = tmp2;}
	if ((tmp1 = mxsy | mhys | mzys) && (tmp2 = getscore(tmp1)) > max_score) {max_mask = tmp1; max_score = tmp2;}
	if (mhyj && (tmp2 = getscore(mhyj)) > max_score) {max_mask = mhyj; max_score = tmp2;}
	if ((tmp1 = mhys | mxsy | mxsx | mdsy | mdsx) && (tmp2 = getscore(tmp1)) > max_score) {max_mask = tmp1; max_score = tmp2;}
	if ((tmp1 = mpph | mhys | mqys) && (tmp2 = getscore(mpph)) > max_score) {max_mask = tmp1; max_score = tmp2;}
	if (mjh && (tmp2 = getscore(mjh)) > max_score) {max_mask = mjh; max_score = tmp2;}

	return max_mask;
}

