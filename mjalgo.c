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
		kpais[j] -= 1;
		kpais[j + 1] -= 1;
		kpais[j + 2] -= 1;
		kpais[9] -= 3;
		success = dfs_shunke(kpais, k);
		kpais[j] += 1;
		kpais[j + 1] += 1;
		kpais[j + 2] += 1;
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
		kpais[j] -= 1;
		kpais[j + 1] -= 1;
		kpais[j + 2] -= 1;
		kpais[9] -= 3;
		success = g_dfs_shunke(kpais,
				       k,
				       p_leftcnt,
				       gpais);
		kpais[j] += 1;
		kpais[j + 1] += 1;
		kpais[j + 2] += 1;
		kpais[9] += 3;
		return success;
	}

	int j1subcnt, j2subcnt;
	if (7 == j) {
		j1subcnt = kpais[j - 1] > 0;
		j2subcnt = kpais[j + 1] > 0;
		needcnt = 2 - j1subcnt - j2subcnt;
		if (needcnt <= *p_leftcnt) {
			kpais[j] -= 1;
			kpais[j - 1] -= j1subcnt;
			kpais[j + 1] -= j2subcnt;
			kpais[9] -= (1 + j1subcnt + j2subcnt);
			*p_leftcnt -= needcnt;
			success = g_dfs_shunke(kpais,
					       k,
					       p_leftcnt,
					       gpais);
			kpais[j] += 1;
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
			kpais[j] -= 1;
			kpais[j - 1] -= j1subcnt;
			kpais[j - 2] -= j2subcnt;
			kpais[9] -= (1 + j1subcnt + j2subcnt);
			*p_leftcnt -= needcnt;
			success = g_dfs_shunke(kpais,
					       k,
					       p_leftcnt,
					       gpais);
			kpais[j] += 1;
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
		kpais[j] -= 1;
		kpais[j + 1] -= j1subcnt;
		kpais[j + 2] -= j2subcnt;
		kpais[9] -= (1 + j1subcnt + j2subcnt);
		*p_leftcnt -= needcnt;
		success = g_dfs_shunke(kpais,
				       k,
				       p_leftcnt,
				       gpais);
		kpais[j] += 1;
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

static int g_filter(pais_t pais, nums_t gnums, int n, nums_t m_gnums)
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

static void g_reset(pais_t pais, nums_t m_gnums, int n)
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

static bool g_check3n(pais_t pais, int gcnt)
{
	int needcnt = 0, remainder;
	for (int i = 0; i < 4; ++i) {
		remainder = pais[i][9] % 3;
		needcnt += remainder == 0 ? remainder : (3 - remainder);
		CHECK_COND(needcnt <= gcnt);
	}
	return true;
}

static bool g_contains(nums_t gnums, int n, int num)
{
	for (int i = 0; i < n; ++i) if (gnums[i] == num) return true;
	return false;
}

bool checkhu_jh(pais_t pais, int *eyei, int *eyej)
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

bool checkhu_pph(pais_t pais, bool jh)
{
	CHECK_COND(jh);

	PAIS_FOREACH(i, j) CHECK_COND(1 != pais[i][j]);
	return true;
}

bool checkhu_hys(pais_t pais, nums_t nums, int n, bool jh)
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

bool checkhu_qxd(pais_t pais)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	PAIS_FOREACH(i, j) CHECK_COND(pais[i][j] == 0 || pais[i][j] == 2);
	return true;
}

bool checkhu_lqd(pais_t pais, bool qxd)
{
	CHECK_COND(qxd && 14 == PAIS_COUNT(pais));

	PAIS_FOREACH(i, j) if (pais[i][j] == 4) return true;
	return false;
}

bool checkhu_qys(pais_t pais, nums_t nums, int n, bool jh)
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

bool checkhu_hyj(pais_t pais, nums_t nums, int n, bool jh)
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

bool checkhu_xsy(pais_t pais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh);

	int kecnt = (pais[3][4] > 2) +
		    (pais[3][5] > 2) +
		    (pais[3][6] > 2);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 33 && nums[i] < NUM_MAX);

	return kecnt > 1 && KIND_ZI == eyei && eyej > 3;
}

bool checkhu_xsx(pais_t pais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh);

	int kecnt = (pais[KIND_ZI][0] > 2) +
		    (pais[KIND_ZI][1] > 2) +
		    (pais[KIND_ZI][2] > 2) +
		    (pais[KIND_ZI][3] > 2);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34);
	return kecnt > 2 && KIND_ZI == eyei && eyej < 4;
}

bool checkhu_zys(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	for (int i = 0; i < KIND_ZI; ++i)
		for(int j = 0; j < 9; ++j) CHECK_COND(pais[i][j] <= 0);
	for (int i = 0; i < n; ++i) CHECK_COND(KIND_ZI == NUM2I(nums[i]));
	return true;
}

bool checkhu_qyj(pais_t pais, nums_t nums, int n, bool jh)
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

bool checkhu_ssy(pais_t pais)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	return pais[0][0] == 1 && pais[1][0] == 1 && pais[2][0] == 1 && pais[3][0] == 1 &&
	       pais[0][8] == 1 && pais[1][8] == 1 && pais[2][8] == 1 && pais[3][8] == 1 &&
	       pais[3][0] >= 1 && pais[3][1] >= 1 && pais[3][2] >= 1 && pais[3][3] >= 1 && pais[3][4] >= 1 && pais[3][5] >= 1 && pais[3][6] >= 1;
}

bool checkhu_dsy(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	int kecnt = (pais[KIND_ZI][4] > 2) +
		    (pais[KIND_ZI][5] > 2) +
		    (pais[KIND_ZI][6] > 2);
	for (int i = 0; i < n; ++i) kecnt += (nums[i] < NUM_MAX && nums[i] > 33);
	return kecnt;
}

bool checkhu_dsx(pais_t pais, nums_t nums, int n, bool jh)
{
	CHECK_COND(jh);

	int kecnt = (pais[KIND_ZI][0] > 2) +
		    (pais[KIND_ZI][1] > 2) +
		    (pais[KIND_ZI][2] > 2) +
		    (pais[KIND_ZI][3] > 2);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34);
	return kecnt == 4;
}

bool g_checkhu_jh(pais_t pais, nums_t gnums, int n, pais_t gpais, int *eyei, int *eyej)
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
			pais[i][j] -= 1;
			pais[i][9] -= 1;
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
			pais[i][j] += 1;
			pais[i][9] += 1;
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

bool g_checkhu_pph(pais_t gpais, bool jh)
{
	CHECK_COND(-1 != gpais[0][0]);
	return checkhu_pph(gpais, jh);
}

bool g_checkhu_hys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh, int eyei)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	bool zi_exist = gpais[KIND_ZI][9] > 0 || eyei == -1;
	bool gconn;
	int kmap[4] = {0, 0, 0, 0}, k;

	for (int i = 0; i < 3; ++i) kmap[i] = gpais[i][9] > 0;

	for (int i = 0; i < n; ++i) {
		k = NUM2I(nums[i]);
		gconn = g_contains(gnums, gn, nums[i]);
		kmap[k] = kmap[k] || !gconn;
		zi_exist = zi_exist || k == KIND_ZI || gconn;
	}
	return zi_exist && kmap[0] + kmap[1] + kmap[2] + kmap[3] < 3;
}

bool g_checkhu_qxd(pais_t pais, nums_t gnums, int n)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	int m_gnums[G_MAX_COUNT * 2] = {-1, -1, -1, -1}; // num|cnt|num|cnt
	int gcnt = g_filter(pais, gnums, n, m_gnums);
	if (gcnt <= 0) return checkhu_qxd(pais);

	bool success = false;
	PAIS_FOREACH(i, j) {
		gcnt -= pais[i][j] == 1 || pais[i][j] == 3;
		success = gcnt >= 0;
		if (!success) goto _failed;
	}

	g_reset(pais, m_gnums, n);
	return true;

_failed:
	g_reset(pais, m_gnums, n);
	return false;
}

bool g_checkhu_qys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	int kmap[4] = {0, 0, 0, 0}, k;
	for (int i = 0; i < 4; ++i) kmap[i] = gpais[i][9] > 0;

	for (int i = 0; i < n; ++i) {
		k = NUM2I(nums[i]);
		kmap[k] = kmap[k] || !g_contains(gnums, gn, nums[i]);
	}

	return kmap[0] + kmap[1] + kmap[2] + kmap[3] == 1;
}

bool g_checkhu_lqd(pais_t pais, nums_t gnums, int n)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	int m_gnums[G_MAX_COUNT * 2] = {-1, -1, -1, -1}; // num|cnt|num|cnt
	int gcnt = g_filter(pais, gnums, n, m_gnums);
	if (gcnt <= 0) return checkhu_lqd(pais, checkhu_qxd(pais));

	bool exist4 = false;
	PAIS_FOREACH(i, j) {
		switch(pais[i][j]) {
			case 1 : gcnt -= 1; break;
			case 3 : gcnt -= 1; exist4 = gcnt >= 0;
			case 4 : exist4 = true;
		}
		if (gcnt < 0) goto _failed;
	}

	g_reset(pais, m_gnums, n);
	return gcnt % 2 == 0 && exist4;

_failed:
	g_reset(pais, m_gnums, n);
	return false;
}

bool g_checkhu_hyj(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh, int eyei)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	bool zi_exist = gpais[KIND_ZI][9] > 0 || -1 == eyei;
	for (int i = 0; i < KIND_ZI; ++i)
		for (int j = 1; j < 8; ++j) CHECK_COND(gpais[i][j] <= 0);

	int k;
	bool gconn;
	for (int i = 0; i < n; ++i) {
		k = NUM2I(nums[i]);
		gconn = g_contains(gnums, gn, nums[i]);
		CHECK_COND(gconn || KIND_ZI == k || 0 == k || 8 == k);
		zi_exist = zi_exist || KIND_ZI == k || gconn;
	}
	return zi_exist;
}

bool g_checkhu_xsy(pais_t gpais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	int kecnt = (gpais[KIND_ZI][4] > 2) +
		    (gpais[KIND_ZI][5] > 2) +
		    (gpais[KIND_ZI][6] > 2);

	for (int i = 0; i < n; ++i) kecnt += nums[i] > 33;
	return kecnt > 1 && (-1 == eyei || (KIND_ZI == eyei && eyej > 3));
}

bool g_checkhu_xsx(pais_t gpais, nums_t nums, int n, bool jh, int eyei, int eyej)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	int kecnt = (gpais[KIND_ZI][0] > 2) +
		    (gpais[KIND_ZI][1] > 2) +
		    (gpais[KIND_ZI][2] > 2) +
		    (gpais[KIND_ZI][3] > 2);

	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34);
	return kecnt > 2 && (-1 == eyei || (KIND_ZI == eyei && eyej < 4));
}

bool g_checkhu_zys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	for (int i = 0; i < KIND_ZI; ++i)
		for (int j = 0; j < 9; ++j) CHECK_COND(gpais[i][j] <= 0);
	for (int i = 0; i < n; ++i) CHECK_COND(KIND_ZI == NUM2I(nums[i]) || g_contains(gnums, gn, nums[i]));
	return true;
}

bool g_checkhu_qyj(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
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

bool g_checkhu_ssy(pais_t pais, nums_t gnums, int n)
{
	CHECK_COND(14 == PAIS_COUNT(pais));

	int m_gnums[G_MAX_COUNT * 2] = {-1, -1, -1, -1}; // num|cnt|num|cnt
	int gcnt = g_filter(pais, gnums, n, m_gnums);

	for (int i = 0; i < KIND_ZI; ++i) {
		if (pais[i][0] > 1 || pais[i][8] > 1) goto _failed;
		gcnt -= ((pais[i][0] == 0) + (pais[i][8] == 0));
		if (gcnt < 0) goto _failed;
	}
	for (int i = 0; i < 7; ++i) if (pais[KIND_ZI][i] > 2) goto _failed;

	g_reset(pais, m_gnums, n);
	return true;

_failed:
	g_reset(pais, m_gnums, n);
	return false;
}

bool g_checkhu_dsy(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(jh && -1 != gpais[0][0]);
	int kecnt = (gpais[3][4] > 2) +
		    (gpais[3][5] > 2) +
		    (gpais[3][6] > 2);
	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 33 || g_contains(gnums, gn, nums[i]));
	return kecnt > 2;
}

bool g_checkhu_dsx(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh)
{
	CHECK_COND(jh && -1 != gpais[0][0]);

	int kecnt = (gpais[3][0] > 2) +
		    (gpais[3][1] > 2) +
		    (gpais[3][2] > 2) +
		    (gpais[3][3] > 2);
	for (int i = 0; i < n; ++i) kecnt += (nums[i] > 29 && nums[i] < 34) || g_contains(gnums, gn, nums[i]);

	return kecnt > 3;
}

bool insert_tingnode(QUEUE* h, int num, int mask)
{
	struct tingnode_s *tnode = malloc(sizeof(struct tingnode_s));
	if (NULL == tnode) goto _nonmem;
	tnode->num = num;
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
	pais_t gpais;
	int eyei, eyej, num, lnum, li, lj;
	bool jh, pph, hys, qxd, qys, lqd, hyj, xsy, xsx, zys, qyj, ssy, dsy, dsx;
	bool hu = false;

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
			jh = g_checkhu_jh(pais, gnums, gn, gpais, &eyei, &eyej) && insert_tingnode(&tmap[num], lnum, JH);
			pph = (PPH & mask) && g_checkhu_pph(gpais, jh) && insert_tingnode(&tmap[num], lnum, PPH);
			hys = (HYS & mask) && g_checkhu_hys(gpais, nums, n, gnums, gn, jh, eyei) && insert_tingnode(&tmap[num], lnum, HYS);
			qxd = (QXD & mask) && g_checkhu_qxd(pais, gnums, n) && insert_tingnode(&tmap[num], lnum, QXD);
			qys = (QYS & mask) && g_checkhu_qys(gpais, nums, n, gnums, gn, jh) && insert_tingnode(&tmap[num], lnum, QYS);
			lqd = (LQD & mask) && g_checkhu_lqd(pais, gnums, n) && insert_tingnode(&tmap[num], lnum, LQD);
			hyj = (HYJ & mask) && g_checkhu_hyj(gpais, nums, n, gnums, gn, jh, eyei) && insert_tingnode(&tmap[num], lnum, HYJ);
			xsy = (XSY & mask) && g_checkhu_xsy(gpais, nums, n, jh, eyei, eyej) && insert_tingnode(&tmap[num], lnum, XSY);
			xsx = (XSX & mask) && g_checkhu_xsx(gpais, nums, n, jh, eyei, eyej) && insert_tingnode(&tmap[num], lnum, XSX);
			zys = (ZYS & mask) && g_checkhu_zys(gpais, nums, n, gnums, gn, jh) && insert_tingnode(&tmap[num], lnum, ZYS);
			qyj = (QYJ & mask) && g_checkhu_qyj(gpais, nums, n, gnums, gn, jh) && insert_tingnode(&tmap[num], lnum, QYJ);
			ssy = (SSY & mask) && g_checkhu_ssy(pais, gnums, n) && insert_tingnode(&tmap[num], lnum, SSY);
			dsy = (DSY & mask) && g_checkhu_dsy(gpais, nums, n, gnums, gn, jh) && insert_tingnode(&tmap[num], lnum, DSY);
			dsx = (DSX & mask) && g_checkhu_dsx(gpais, nums, n, gnums, gn, jh) && insert_tingnode(&tmap[num], lnum, DSX);
			hu = hu || jh || pph || hys || qxd || qys || lqd || hyj || xsy || xsx || zys || qyj || ssy || dsy || dsx;
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
	bool jh;

	jh = g_checkhu_jh(pais, gnums, gn, gpais, &eyei, &eyej);
	return  (jh ? JH : 0) |
		((PPH & mask) && g_checkhu_pph(gpais, jh) ? PPH : NONE) |
		((HYS & mask) && g_checkhu_hys(gpais, nums, n, gnums, gn, jh, eyei) ? HYS : NONE) |
		((QXD & mask) && g_checkhu_qxd(pais, gnums, n) ? QXD : NONE) |
		((QYS & mask) && g_checkhu_qys(gpais, nums, n, gnums, gn, jh) ? QYS : NONE) |
		((LQD & mask) && g_checkhu_lqd(pais, gnums, n) ? LQD : NONE) |
		((HYJ & mask) && g_checkhu_hyj(gpais, nums, n, gnums, gn, jh, eyei) ? HYJ : NONE) |
		((XSY & mask) && g_checkhu_xsy(gpais, nums, n, jh, eyei, eyej) ? XSY : NONE) |
		((XSX & mask) && g_checkhu_xsx(gpais, nums, n, jh, eyei, eyej) ? XSX : NONE) |
		((ZYS & mask) && g_checkhu_zys(gpais, nums, n, gnums, gn, jh) ? ZYS : NONE) |
		((QYJ & mask) && g_checkhu_qyj(gpais, nums, n, gnums, gn, jh) ? QYJ : NONE) |
		((SSY & mask) && g_checkhu_ssy(pais, gnums, n) ? SSY : NONE) |
		((DSY & mask) && g_checkhu_dsy(gpais, nums, n, gnums, gn, jh) ? DSY : NONE) |
		((DSX & mask) && g_checkhu_dsx(gpais, nums, n, gnums, gn, jh) ? DSX : NONE);
}

