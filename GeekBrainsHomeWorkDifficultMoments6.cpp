#include <iostream>
#include<thread>
#include<chrono>
#include <mutex>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

// Task 1

mutex mtx;

void pcout(const char a) {
	unique_lock<mutex> stop(mtx, defer_lock);
	stop.lock();
	cout << "pcout asked from\t" << this_thread::get_id() << endl;


	cout << "Thread Id\t" << this_thread::get_id() << "\tworking\n";
	this_thread::sleep_for(chrono::milliseconds(1000));
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t i = 0; i < 5; i++)
		{
			cout << a << " ";
			this_thread::sleep_for(chrono::milliseconds(20));
		}
		cout << endl;
	}
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "Thread Id\t" << this_thread::get_id() << "\tfinished\n";
	stop.unlock();
}

//Task 2

int SimpleNumber(int n) {
	lock_guard<mutex>LG{ mtx };


	int size = n; // число элементов для массива чисел для просеивания
	int* primes = new int[n]; // массив простых чисел
	int* numbers = new int[size]; // массив для чисел

	for (int i = 0; i < size; i++)
		numbers[i] = i; // заполняем массив (число равно индексу элемента)

	primes[0] = 2; // первое простое число - 2
	int i = 0; // индекс текущего простого числа
	while (i < n) {
		int p = primes[i++]; // запоминаем текущее простое число


		for (int j = p * 2; j < size; j += p)
			numbers[j] = 0; // обнуляем все кратные ему числа в массиве

		while (numbers[p + 1] == 0)
			p++; // ищем следующее ненулевое число

		if (p + 1 >= size) { // если выйдем за границы, расширяем массив
			int* tmp = new int[size * 2];

			for (int k = 0; k < size; k++) {
				tmp[k] = numbers[k];
			}
			delete[] numbers;

			size *= 2;
			numbers = tmp;

			for (int j = size / 2; j < size; j++)
				numbers[j] = j; // заполняем новую часть массива числами

			i = 0; // возвращаемся к начальной стадии просеивания
		}
		else
			primes[i] = p + 1; // запоминаем новое простое число

	}
	delete[] numbers;
	return primes[n - 1];
}

//Task 3

mutex mtx1;
mutex mtx2;
class House
{
	vector<int>Items{ 1000,2000,3500,10000,999999,1500,2222,50 };
public:
	House() {};
	House(int a) {
		Items.push_back(a);
	};
	void Print() {
		mtx1.lock();
		mtx2.lock();

		cout << "\tPrices of all Items :\n";
		copy(Items.begin(), Items.end(), ostream_iterator<int>(cout, "p "));
		mtx1.unlock();
		mtx2.unlock();
	}
	void Add(int n) {
		mtx1.lock();
		mtx2.lock();

		cout << "\tAdded an item worth :\n" << n << "p" << endl;
		Items.push_back(n);

		mtx1.unlock();
		mtx2.unlock();
	}
	void Steal() {
		mtx1.lock();
		mtx2.lock();

		cout << "\tAn item worth was stolen :\n";
		auto it = std::max_element(Items.begin(), Items.end());
		cout << *it;
		Items.erase(it);

		mtx1.unlock();
		mtx2.unlock();

	}

};



int main()
{
	cout << "Begin Task 1" << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));

	char SomeInterestSymbol1 = '$';
	char SomeInterestSymbol2 = '%';

	thread th(pcout, SomeInterestSymbol1);
	thread th2(pcout, SomeInterestSymbol2);

	th.join();
	th2.join();

	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "Task 1 complete" << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "Begin Task 2" << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));

	int SearchNum = 1000000;
	thread th3([&]() {
		cout << "\tSimple Number started" << endl;
		this_thread::sleep_for(chrono::milliseconds(200));
		cout << "\tSimple Number searching numbers" << endl;

		cout << "\tSimple Number found number " << endl;
		});
	thread th4([&]() {

		int x = SimpleNumber(SearchNum);
		cout << SearchNum << " Simple num is :" << x << endl;
		});

	th3.join();
	th4.join();

	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "Task 2 complete" << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "Begin Task 3" << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));

	House H;

	thread Owner([&]() {
		for (size_t i = 0; i < 10; i++)
		{
			this_thread::sleep_for(chrono::milliseconds(3000));
			H.Add(((500 + rand()) / 100) * 100);
			H.Print();
			cout << endl;
		}});

	thread Thief([&]() {
		for (size_t i = 0; i < 10; i++)
		{
			this_thread::sleep_for(chrono::milliseconds(1500));
			H.Steal();
			cout << endl;
		}});


	Owner.join();
	Thief.join();
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "Task 3 complete" << endl;

}