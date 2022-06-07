#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double Float;

inline Float Sqr(Float v) { return v * v; }	// ������� �����

unsigned int gRndSeed = 1;	// ��������� ����. �����

// ������� gRndSeed
inline void SRnd(unsigned int seed)
{
	gRndSeed = (seed == 0) ? 1 : seed;
}

// ����������� ����. �����
inline unsigned int RndI()
{
	return gRndSeed = (16807L * gRndSeed) % 2147483647L;
}

// ����������� ����. �����
inline unsigned int RndU()
{
	return gRndSeed = gRndSeed * 1664525L + 1013904223L;
}

// ����������� ����. ����� �� 0 �� 1
inline Float Rnd()
{
	return Float(RndU()) / Float(0xffffffff);
}

// �������� ��������� �����
Float RndG()
{
	static int was = 0;	// ���� �� ��������� ���� �����
	static Float r = 0;	// ���������� ����. ����� �� ����
	if (was) { was = 0; return r; }	// r �� ���������� ����������

	Float s, v1, v2;
	// �������� ��������� � ����
	do
	{
		v1 = 2 * Rnd() - 1;		// ����� � �������� [-1..1]
		v2 = 2 * Rnd() - 1;
		s = Sqr(v1) + Sqr(v2);	// ������� ���������� �� ������
	} while (s >= 1.0 || s == 0);

	was = 1;
	r = v1 * sqrt(-2 * log(s) / s);	// ������ �����
	return v2 * sqrt(-2 * log(s) / s); // ������������ ������ �����
}