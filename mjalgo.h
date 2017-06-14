/*************************************************************************
FileName: mjalgo.h
Author: turbobhh
Mail: turbobhh@gmail.com
CreatedTime: Thu 08 Jun 2017 08:37:42 PM CST
 ************************************************************************/
#ifndef _BH_HEADERMJALGO_H_
#define _BH_HEADERMJALGO_H_
#if defined (__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "queue.h"

#define NUM2I(num) ((num) / 10)
#define NUM2J(num) ((num) % 10)
#define IJ2NUM(i, j) ((i) * 10 + (j))
#define NUM_MAX 37

#define KIND_ZI 3
#define G_MAX_COUNT 2

#define PAIS_COPY(dst, src) memcpy(dst, src, sizeof(int) * 4 * 10)
#define PAIS_FOREACH(i, j) for (int i = 0; i < 4; ++i) for (int j = 0; j < 9; ++j)
#define PAIS_INSERT(pais, i, j, n) do { pais[i][j] += (n); pais[i][9] += (n); } while(0)
#define PAIS_DELETE(pais, i, j, n) do { pais[i][j] -= (n); pais[i][9] -= (n); } while(0)
#define PAIS_COUNT(pais) (pais[0][9] + pais[1][9] + pais[2][9] + pais[3][9])

#define JH   0x1
#define PPH  0x2
#define HYS  0x4
#define QXD  0x8
#define LQD  0x10
#define QYS  0x20
#define HYJ  0x40
#define XSY  0x80
#define XSX  0x100
#define ZYS  0x200
#define QYJ  0x400
#define SSY  0x800
#define DSY  0x1000
#define DSX  0x2000
#define ALL  0x3FFF
#define NONE 0x0

	typedef int pais_t[4][10];
	typedef int kpais_t[10];
	typedef int nums_t[NUM_MAX];

	/* *
	 * pais : 手牌
	 * nums : 碰或杠的牌
	 * n    : nums数组大小
	 * jh   : 是否鸡胡
	 * eyei : 将牌花色
	 * eyej : 将牌数值
	 * */
	bool checkhu_jh(pais_t pais, int *eyei, int *eyej);
	bool checkhu_pph(pais_t pais, bool jh, int n);
	bool checkhu_hys(pais_t pais, nums_t nums, int n, bool jh);
	bool checkhu_qxd(pais_t pais);
	bool checkhu_lqd(pais_t pais, bool qxd);
	bool checkhu_qys(pais_t pais, nums_t nums, int n, bool jh);
	bool checkhu_hyj(pais_t pais, nums_t nums, int n, bool jh);
	bool checkhu_xsy(pais_t pais, nums_t nums, int n, bool jh, int eyei, int eyej);
	bool checkhu_xsx(pais_t pais, nums_t nums, int n, bool jh, int eyei, int eyej);
	bool checkhu_zys(pais_t pais, nums_t nums, int n, bool jh);
	bool checkhu_qyj(pais_t pais, nums_t nums, int n, bool jh);
	bool checkhu_ssy(pais_t pais);
	bool checkhu_dsy(pais_t pais, nums_t nums, int n, bool jh);
	bool checkhu_dsx(pais_t pais, nums_t nums, int n, bool jh);

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
	bool g_checkhu_jh(pais_t pais, nums_t gnums, int n, pais_t gpais, int *eyei, int *eyej);
	bool g_checkhu_pph(pais_t gpais, bool jh, int n);
	bool g_checkhu_qxd(pais_t pais, nums_t gnums, int gn, pais_t gpais);
	bool g_checkhu_lqd(pais_t pais, nums_t gnums, int gn, pais_t gpais);
	bool g_checkhu_hys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh, int eyei, bool qdh);
	bool g_checkhu_qys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool hu, bool qdh);
	bool g_checkhu_hyj(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh, int eyei);
	bool g_checkhu_xsy(pais_t gpais, nums_t nums, int n, bool jh, int eyei, int eyej);
	bool g_checkhu_xsx(pais_t gpais, nums_t nums, int n, bool jh, int eyei, int eyej);
	bool g_checkhu_zys(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh);
	bool g_checkhu_qyj(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh);
	bool g_checkhu_ssy(pais_t pais, nums_t gnums, int gn);
	bool g_checkhu_dsy(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh);
	bool g_checkhu_dsx(pais_t gpais, nums_t nums, int n, nums_t gnums, int gn, bool jh);

	typedef QUEUE tingmap_t[NUM_MAX];
	typedef uint32_t humask_t;

	struct tingnode_s {
		QUEUE node;
		int num;
		int score;
		humask_t mask;
	};

#define TINGMAP_INIT(tmap) for (int i = 0; i < NUM_MAX; ++i) QUEUE_INIT(&tmap[i])
#define TINGMAP_FREE(tmap)									\
	do {											\
		for (int i = 0; i < NUM_MAX; ++i) {						\
			QUEUE *h = &tmap[i];							\
			QUEUE *q = QUEUE_HEAD(h);						\
			while (q != h) {							\
				struct tingnode_s *p = QUEUE_DATA(q, struct tingnode_s, node);	\
				q = QUEUE_NEXT(q);						\
				QUEUE_REMOVE(&(p->node));					\
				free(p);							\
			}									\
		}										\
	} while (0)

inline static int comp(const void *pa, const void *pb)
{
	struct tingnode_s *tnode1 = *(struct tingnode_s **)pa;
	struct tingnode_s *tnode2 = *(struct tingnode_s **)pb;

	if (tnode1->score != tnode2->score) return tnode2->score - tnode1->score;
	if (tnode1->num != tnode2->num) return tnode1->num - tnode2->num;
	return tnode1->mask - tnode2->mask;
}
#define TINGMAP_FOREACH_BEGEIN(tmap, num, tnodes, n)						\
	for (int num = 0; num < NUM_MAX; ++num) {						\
		QUEUE* h = &tmap[num];								\
		if (QUEUE_EMPTY(h)) continue;							\
		int n = QUEUE_LEN(h);								\
		struct tingnode_s *tnodes[n];							\
		int idx = -1;									\
		QUEUE *q;									\
		QUEUE_FOREACH(q, h) {								\
			struct tingnode_s *tnode = QUEUE_DATA(q, struct tingnode_s, node);	\
			tnodes[++idx] = tnode;							\
		}										\
		qsort(tnodes, n, sizeof(tnodes[0]), comp);
#define TINGMAP_FOREACH_END }
	/* *
	 * pais  : 手牌
	 * gnums : 鬼牌
	 * lnums : 剩余牌
	 * ln    : lnums数组大小
	 * nums  : 碰或杠的牌
	 * n     : nums数组
	 * mask  : 胡牌掩码
	 * hs    : 出牌对应的胡牌列表
	 * */
	bool g_gettingdata(pais_t pais,
			   nums_t gnums, int gn,
			   nums_t lnums, int ln,
			   nums_t nums , int n,
			   humask_t mask,
			   tingmap_t tdata);

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

	inline static humask_t g_gethumask(pais_t pais,
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
		if (mpph && (tmp2 = getscore(mpph)) > max_score) {max_mask = mpph; max_score = tmp2;}
		if (mjh && (tmp2 = getscore(mjh)) > max_score) {max_mask = mjh; max_score = tmp2;}

		return max_mask;
	}

#if defined (__cplusplus)
}
#endif
#endif

