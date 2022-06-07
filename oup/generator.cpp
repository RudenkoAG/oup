#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double Float;

inline Float Sqr(Float v) { return v * v; }	// квадрат числа

unsigned int gRndSeed = 1;	// последнее случ. число

// задание gRndSeed
inline void SRnd(unsigned int seed)
{
	gRndSeed = (seed == 0) ? 1 : seed;
}

// равномерное случ. число
inline unsigned int RndI()
{
	return gRndSeed = (16807L * gRndSeed) % 2147483647L;
}

// равномерное случ. число
inline unsigned int RndU()
{
	return gRndSeed = gRndSeed * 1664525L + 1013904223L;
}

// равномерное случ. число от 0 до 1
inline Float Rnd()
{
	return Float(RndU()) / Float(0xffffffff);
}

// гауссово случайное число
Float RndG()
{
	static int was = 0;	// была ли вычислена пара чисел
	static Float r = 0;	// предыдущее случ. число из пары
	if (was) { was = 0; return r; }	// r из предыдущих вычислений

	Float s, v1, v2;
	// ожидание попадания в круг
	do
	{
		v1 = 2 * Rnd() - 1;		// точка в квадрате [-1..1]
		v2 = 2 * Rnd() - 1;
		s = Sqr(v1) + Sqr(v2);	// квадрат расстояния от центра
	} while (s >= 1.0 || s == 0);

	was = 1;
	r = v1 * sqrt(-2 * log(s) / s);	// первое число
	return v2 * sqrt(-2 * log(s) / s); // возвращаемое второе число
}