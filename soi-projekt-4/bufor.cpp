#include <iostream>
#include "monitor.h"
#include <deque>

#define BUFFER_MAX_SIZE 9
#define BUFFER_MIN_SIZE 3
#define PRODUCER_SLEEP_MIN 1
#define EVEN_CONSUMER 2
#define ODD_CONSUMER 1

class EnhancedMonitor : public Monitor
{
private:
	std::deque<int> buffer;
	int waitingProducers, waitingEvenConsumers, waitingOddConsumers;
	Condition producer, evenConsumer, oddConsumer;

public:
	EnhancedMonitor();
	void addElement(int numToAdd);
	int removeElement(int whichConsumer);
};

EnhancedMonitor myMonitor;

EnhancedMonitor::EnhancedMonitor()
{
	waitingProducers = 0;
	waitingEvenConsumers = 0;
	waitingOddConsumers = 0;
}

void EnhancedMonitor::addElement(int numToAdd)
{
	enter();
	if (buffer.size() >= BUFFER_MAX_SIZE)
	{
		waitingProducers++;
		wait(producer);
		waitingProducers--;
	}

	buffer.push_back(numToAdd);
	std::cout << numToAdd << " added to buffer." << std::endl;

	if (buffer.size() < BUFFER_MAX_SIZE && waitingProducers > 0)
	{
		signal(producer);
	}
	else if (buffer.size() >= BUFFER_MIN_SIZE && buffer.front() % 2 == 0 && waitingEvenConsumers > 0)
	{
		signal(evenConsumer);
	}
	else if (buffer.size() >= BUFFER_MIN_SIZE && buffer.front() % 2 == 1 && waitingOddConsumers > 0)
	{
		signal(oddConsumer);
	}
	else
	{
	}
	leave();
}

int EnhancedMonitor::removeElement(int whichConsumer)
{
	enter();
	if (whichConsumer == ODD_CONSUMER)
	{
		if (buffer.size() < BUFFER_MIN_SIZE || buffer.front() % 2 == 0)
		{
			waitingOddConsumers++;
			wait(oddConsumer);
			waitingOddConsumers--;
		}

		std::cout << buffer.front() << " will be deleted from buffer by odd consumer." << std::endl;
		buffer.pop_front();

		if (buffer.size() < BUFFER_MAX_SIZE && waitingProducers > 0)
		{
			signal(producer);
		}
		else if (buffer.size() >= BUFFER_MIN_SIZE && buffer.front() % 2 == 0 && waitingEvenConsumers > 0)
		{
			signal(evenConsumer);
		}
		else if (buffer.size() >= BUFFER_MIN_SIZE && buffer.front() % 2 == 1 && waitingOddConsumers > 0)
		{
			signal(oddConsumer);
		}
		else
		{
		}
		leave();
	}
	if (whichConsumer == EVEN_CONSUMER)
	{
		if (buffer.size() < BUFFER_MIN_SIZE || buffer.front() % 2 != 0)
		{
			waitingEvenConsumers++;
			wait(evenConsumer);
			waitingEvenConsumers--;
		}

		std::cout << buffer.front() << " will be deleted from buffer by even consumer." << std::endl;
		buffer.pop_front();

		if (buffer.size() < BUFFER_MAX_SIZE && waitingProducers > 0)
		{
			signal(producer);
		}
		else if (buffer.size() >= BUFFER_MIN_SIZE && buffer.front() % 2 == 0 && waitingEvenConsumers > 0)
		{
			signal(evenConsumer);
		}
		else if (buffer.size() >= BUFFER_MIN_SIZE && buffer.front() % 2 == 1 && waitingOddConsumers > 0)
		{
			signal(oddConsumer);
		}
		else
		{
		}
		leave();
	}
}

void * producer( void * )
{
	std::cout << "New producer created!" << std::endl;
	while (true)
	{
		int randNum = rand();
		std::cout << randNum << " produced." << std::endl;
		myMonitor.addElement(randNum);
		sleep(PRODUCER_SLEEP_MIN + rand() % 2);
	}
}

void * oddConsumer( void * )
{
	 std::cout << "New odd consumer created!" << std::endl;
	 while (true)
	 {
		 myMonitor.removeElement(1);
		 std::cout << "Odd consumer ended consumption." << std::endl;
	 }
}

void * evenConsumer(void * )
{
	 std::cout << "New even consumer created!" << std::endl;
	 while (true)
	 {
		 myMonitor.removeElement(2);
		 std::cout << "Even consumer ended consumption." << std::endl;
	 }
 }

int main()
{
	pthread_t prod1, prod2, evenCons, oddCons;
	char a = 'A';
	char b = 'B';
	pthread_create(&prod1, NULL, producer, &a);
	pthread_create(&evenCons, NULL, evenConsumer, &a);
	pthread_create(&prod2, NULL, producer, &b);
	pthread_create(&oddCons, NULL, oddConsumer, &b);

	pthread_join(prod1, NULL);
	pthread_join(evenCons, NULL);
	pthread_join(prod2, NULL);
	pthread_join(oddCons, NULL);
	return 0;
}

