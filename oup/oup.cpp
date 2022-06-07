// Необходимо добавить в свойства проекта следующие строки
// C/C++ -> Препроцессор -> Определения препроцессора -> ;_CRT_SECURE_NO_WARNINGS
// Компоновщик -> Командная строка -> /FORCE:MULTIPLE
#include "pch.h"
#include "generator.cpp"
#include <Windows.h>
#include <time.h>
#include <iostream>

using namespace std;

void graphs(Float a1, Float b1, Float x1, Float volat, Float num, Float lag)
{
	Float step = 0.1;	// шаг
	int separatedPart = (int)(num / 2.7);
	Float alpha = 0;// равновесное значение
	Float beta = 0;	// параметр сжатия к alpha
	Float *numbers = new Float[num];	// массив с num элементами
	Float dt = step / lag;	// шаг по времени
	Float sqrt_dt = sqrt(dt);	// корень из dt
	Float x = 1;	// начальное значение
	Float t = 0;	// момент времени

	Float sepMax = 0;	// максимальное значение в отсеченной части
	int sepMaxNum = 0;	// номер элемента
	Float opMax = 0;	// значение оптимального элемента
	int opMaxNum = 0;	
	Float genMax = 0;	// наибольшее значение из всех элементов
	int genMaxNum = 0;

	int lastPointX = 0;	// значения X и Y точек
	int lastPointY = 0;
	int tries = 1;		// номер попытки

	// создание кистей для графиков
	HDC hDC = GetDC(GetConsoleWindow());
	HPEN Pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	// красный для соединения точек
	HPEN DotsPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	// синий для отображения отсеченной части
	HPEN sepPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 255));
	// зеленый для оптимального элемента
	HPEN opElPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	// серый для максимального значения в отсеченной части
	HPEN sepMaxPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
	// темно-зеленый для максимального значения из всех
	HPEN genMaxPen = CreatePen(PS_SOLID, 2, RGB(0, 100, 0));
	system("mode con cols=70 lines=60");

	SRnd((unsigned)time(0));

	FILE *out = fopen("graphs.out", "w");
	fprintf(out, "dx = -1 * %f * (%f - %f)dt + %f * dW \n num = %f \n lag = %f \n", b1, x1, a1, volat, num, lag);
	do {
		InvalidateRect(NULL, NULL, TRUE);
		system("cls");

		x = x1;
		t = 0;
		alpha = a1;
		beta = b1;
		sepMax = 0;
		opMax = 0;
		genMax = 0;
		sepMaxNum = 0;
		opMaxNum = 0;
		genMaxNum = 0;
		lastPointX = 0;
		lastPointY = 650;

		SelectObject(hDC, Pen);
		MoveToEx(hDC, 0, 650, NULL);
		LineTo(hDC, 500, 650);
		MoveToEx(hDC, 1, 0, NULL);
		LineTo(hDC, 1, 650);
		SelectObject(hDC, DotsPen);

		fprintf(out, "\n %i-й набор значений \n", tries);

		// Стохастический процесс dx = -β(x - α)dt + b(x,t)δW
		// x притягивается к уровню α
		// если x > α, то процесс идет вниз
		// при опускании x ниже α снос будет положительным и потянет процесс вверх
		// параметр β > 0  характеризует величину "силы притяжения" к равновесному значению α
		for (int k = 1; k <= num; k++)
		{
			for (int j = 0; j < lag; j++, t += dt)
				x += -1 * beta * (x - alpha)*dt + volat * RndG()*sqrt_dt;
			numbers[k - 1] = x;
			fprintf(out, "%g\t%g\n", t, x);
			fflush(out);
		}

		// нахождение номеров искомых элементов для отображения на графике
		// наибольший элемент из всех
		for (int i = 0; i < num; i++)
		{
			if (numbers[i] >= genMax)
			{
				genMax = numbers[i];
				genMaxNum = i;
			}
		}

		// наибольший элемент в отсеченной части
		for (int i = 0; i < separatedPart; i++)
		{
			if (numbers[i] >= sepMax)
			{
				sepMax = numbers[i];
				sepMaxNum = i;
			}
		}

		// оптимальный элемент
		for (int i = separatedPart; i < num; i++)
		{
			if (numbers[i] >= sepMax)
			{
				opMax = numbers[i];
				opMaxNum = i;
				break;
			}
		}

		// построение графика
		for (x = 0.0f; x < num; x += 1.00f)
		{
			// наибольший элемент в отсеченной части
			if ((int)x == sepMaxNum)
			{
				SelectObject(hDC, sepMaxPen);
				MoveToEx(hDC, 5 * 100 / num * x + 1, 650, NULL);
				LineTo(hDC, 5 * 100 / num * x + 1, 1);
				SelectObject(hDC, DotsPen);
				MoveToEx(hDC, lastPointX, lastPointY, NULL);
			}

			// отсеченная часть
			if ((int)x == separatedPart)
			{
				SelectObject(hDC, sepPen);
				MoveToEx(hDC, 5 * 100 / num * x + 1, 650, NULL);
				LineTo(hDC, 5 * 100 / num * x + 1, 1);
				SelectObject(hDC, DotsPen);
				MoveToEx(hDC, lastPointX, lastPointY, NULL);
			}

			// наибольший элемент из всех
			if ((int)x == genMaxNum)
			{
				SelectObject(hDC, genMaxPen);
				MoveToEx(hDC, 5 * 100 / num * x + 1, 650, NULL);
				LineTo(hDC, 5 * 100 / num * x + 1, 1);
				SelectObject(hDC, DotsPen);
				MoveToEx(hDC, lastPointX, lastPointY, NULL);
			}

			// оптимальный элемент
			if ((int)x == opMaxNum && (int)x >= separatedPart)
			{
				SelectObject(hDC, opElPen);
				MoveToEx(hDC, 5 * 100 / num * x + 1, 650, NULL);
				LineTo(hDC, 5 * 100 / num * x + 1, 1);
				SelectObject(hDC, DotsPen);
				MoveToEx(hDC, lastPointX, lastPointY, NULL);
			}

			LineTo(hDC, 5 * 100 / num * x + 1, -numbers[(int)x] * 350 / alpha + 650);
			MoveToEx(hDC, 5 * 100 / num * x + 1, -numbers[(int)x] * 350 / alpha + 650, NULL);
			lastPointX = 5 * 100 / num * x + 1;
			lastPointY = -numbers[(int)x] * 350 / alpha + 650;
		}
		tries++;
	} while (std::cin.get() == '\n');
	fclose(out);
}

void endless_calculations(Float a1, Float b1, Float x1, Float volat, Float num, Float lag, int total_tries)
{
	Float step = 0.1;	// шаг
	int separatedPart = (int)(num / 2.7);
	Float alpha = 0;// равновесное значение
	Float beta = 0;	// параметр сжатия к alpha
	Float *numbers = new Float[num];	// массив с num элементами
	Float dt = step / lag;	// шаг по времени
	Float sqrt_dt = sqrt(dt);	// корень из dt
	Float x = 1;	// начальное значение
	Float t = 0;	// момент времени

	Float sepMax = 0;	// максимальное значение в отсеченной части
	int sepMaxNum = 0;	// номер элемента
	Float opMax = 0;	// значение оптимального элемента
	int opMaxNum = 0;
	Float genMax = 0;	// наибольшее значение из всех элементов
	int genMaxNum = 0;

	int numberOfCalculation = 0;// кол-во завершенных подсчетов

	double Tries = 0;	// кол-во попыток в одном подсчете
	double Wins = 0;	// успешные использования стратегии
	double Winrate = 0;	// процент "побед"

	int lastPointX = 0;
	int lastPointY = 0;

	int total_i_tries = total_tries;
	int one_percent = total_i_tries / 100;

	SRnd((unsigned)time(0));

	FILE *out = fopen("calculations.out", "w");
	fprintf(out, "dx = -1 * %f * (%f - %f)dt + %f * dW \n num = %f \n lag = %f \n Tries = %d \n", b1, x1, a1, volat, num, lag, total_tries);
	for (;;) {
		// вывод на экран кол-ва завершенных подсчетов и текущего процента выполнения
		for (int total_i = 0; total_i < total_i_tries; total_i++)
		{
			if (total_i_tries >= 100)
			{
				if (total_i % one_percent == 0)
				{
					system("cls");
					printf("Finished calculations: %i\n", numberOfCalculation);
					printf("%i\n", total_i / one_percent);
				}
			}

			x = x1;
			alpha = a1;
			beta = b1;
			t = 0;
			sepMax = 0;
			opMax = -1;
			genMax = 0;
			sepMaxNum = 0;
			opMaxNum = 0;
			genMaxNum = 0;

			// Стохастический процесс dx = -β(x - α)dt + b(x,t)δW
			// x притягивается к уровню α
			// если x > α, то процесс идет вниз
			// при опускании x ниже α снос будет положительным и потянет процесс вверх
			// параметр β > 0  характеризует величину "силы притяжения" к равновесному значению α
			for (int k = 1; k <= num; k++)
			{
				for (int j = 0; j < lag; j++, t += dt)
					x += -1 * beta * (x - alpha)*dt + volat * RndG()*sqrt_dt;
				numbers[k - 1] = x;
			}

			// нахождение номеров искомых элементов для отображения на графике
			// наибольший элемент из всех
			for (int i = 0; i < num; i++)
			{
				if (numbers[i] >= genMax)
				{
					genMax = numbers[i];
					genMaxNum = i;
				}
			}

			// наибольший элемент в отсеченной части
			for (int i = 0; i < separatedPart; i++)
			{
				if (numbers[i] >= sepMax)
				{
					sepMax = numbers[i];
					sepMaxNum = i;
				}
			}

			// оптимальный элемент
			for (int i = separatedPart; i < num; i++)
			{
				if (numbers[i] >= sepMax || i == num - 1)
				{
					opMax = numbers[i];
					opMaxNum = i;
					break;
				}
			}

			// подсчет процента "побед"
			if (opMax == genMax)
				Wins += 1;
			Tries += 1;
			Winrate = 100 * Wins / Tries;
		}
		numberOfCalculation += 1;
		system("cls");
		printf("Finished calculations: %i\n", numberOfCalculation);
		fprintf(out, "\n%f%%", Winrate);
		fflush(out);
		Tries = 0;
		Wins = 0;
		Winrate = 0;
	}
}


void main()
{
	int operation;
	Float a1, b1, x1, volat1, num1, lag1;
	cout << "Operation:" << endl << "1 - graphs" << endl << "2 - general calculation" << endl;
	cin >> operation;

	if (operation != 1 && operation != 2 )
	{
		do
		{
			system("cls");
			cout << "Operation:" << endl << "1 - graphs" << endl << "2 - general calculation" << endl;
			cin >> operation;
		} while (operation != 1 && operation != 2);
	}

	system("cls");

	cout << "dx = -1 * beta * (x - alpha) * dt + b(x, t) * dW" << endl;
	cout << "beta = ";
	cin >> b1;
	system("cls");
	cout << "dx = -1 * " << b1 << " * (x - alpha) * dt + b(x, t) * dW" << endl;
	cout << "x = ";
	cin >> x1;
	system("cls");
	cout << "dx = -1 * " << b1 << " * (" << x1 << " - alpha) * dt + b(x, t) * dW" << endl;
	cout << "alpha = ";
	cin >> a1;
	system("cls");
	cout << "dx = -1 * " << b1 << " * (" << x1 << " - " << a1 << ") * dt + b(x, t) * dW" << endl;
	cout << "b(x, t) = ";
	cin >> volat1;
	system("cls");
	cout << "dx = -1 * " << b1 << " * (" << x1 << " - " << a1 << ") * dt + " << volat1 << " * dW" << endl;
	cout << "num = ";
	cin >> num1;
	cout << "lag = ";
	cin >> lag1;
	cin.ignore(256, '\n');

	switch (operation)
	{
	case 1:
		graphs(a1, b1, x1, volat1, num1, lag1);
		break;
	case 2:
		int number_of_tries;
		cout << "Tries = ";
		cin >> number_of_tries;
		endless_calculations(a1, b1, x1, volat1, num1, lag1, number_of_tries);
		break;
	}
}