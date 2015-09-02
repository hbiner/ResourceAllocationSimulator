// ResourceAllocationPolicy.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <stdlib.h>
#include<windows.h>
#include "ResourceAllocationPolicy.h"
using namespace std;

//��ʼ��
void ResourceAllocationPolicy::Init()
{
	//��ʼ����ʼ��Դ
	m_initResource[0].cpus = m_resource[0].cpus = 28;
	m_initResource[0].ram = m_resource[0].ram = 28;
	m_initResource[0].bandwidth = m_resource[0].bandwidth = 28;
	m_initResource[0].storage = m_resource[0].storage = 28;
	m_initResource[1].cpus = m_resource[1].cpus = 29;
	m_initResource[1].ram = m_resource[1].ram = 29;
	m_initResource[1].bandwidth = m_resource[1].bandwidth = 29;
	m_initResource[1].storage = m_resource[1].storage = 29;
	m_initResource[2].cpus = m_resource[2].cpus = 33;
	m_initResource[2].ram = m_resource[2].ram = 33;
	m_initResource[2].bandwidth = m_resource[2].bandwidth = 33;
	m_initResource[2].storage = m_resource[2].storage = 33;
	//������������
	GenerateRequest();
	//��ǰ10���������request buffer��
	for (int i = 0; i < 10; i++)
	{
		m_vecRequestBuffer.push_back(m_request[i]);
	}
}

//��̬��������
void ResourceAllocationPolicy::GenerateRequest()
{
	/*int iSumCpus = 0, iSumRam = 0, iSumBandwidth = 0, iSumStorage = 0;
	for (int i = 0; i < RESOURCE_COUNT; i++)
	{
		iSumCpus += m_resource[i].cpus;
		iSumRam += m_resource[i].ram;
		iSumBandwidth += m_resource[i].bandwidth;
		iSumStorage += m_resource[i].storage;
	}*/
	cout << "--------------ALL REQUEST---------------\n";
	for (int i = 0; i < REQUEST_COUNT; i++)
	{
		int iCpus = RandomNum(8) + 1;
		int iRam = RandomNum(8) + 1;
		int iBandwidth = RandomNum(8) + 1;
		int iStorage = RandomNum(8) + 1;
		int iEndTime = RandomNum(10) + 1 + i;
		m_request[i].index = i;
		m_request[i].cpus = iCpus;
		m_request[i].ram = iRam;
		m_request[i].bandwidth = iBandwidth;
		m_request[i].storage = iStorage;
		if (i < 10)
		{
			m_request[i].startTime = 1;
			m_request[i].endTime = 2;
		}
		else
		{
			m_request[i].startTime = i / 5;
			m_request[i].endTime = (i / 5);
		}
		cout << i << " : " << iCpus << "\t" << iRam << "\t" << iBandwidth << "\t" << iStorage << "\t" << m_request[i].startTime << "\t" << m_request[i].endTime << endl;
	}
}

//����[0,num]��������
int ResourceAllocationPolicy::RandomNum(int num)
{
	if (num == 0)
		return 0;
	int iRand = rand() / (RAND_MAX / num);
	return iRand;
}

//��ʼ����Դyѡȡ���������
void ResourceAllocationPolicy::InitFillRequestToCurY(int i)	
{
	while (true)
	{
		int iCur = RandomNum(iRequestBufferSize - 1);	//�������request����ǰ��Դy��
		if (setCurY.count(m_vecRequestBuffer[iCur].index) == 0 && setCurY.size() < iRequestBufferSize)//��request����setCurY�У���setCurY��δ��������������е�requests
		{
			iSumCpus += m_vecRequestBuffer[iCur].cpus;
			iSumRam += m_vecRequestBuffer[iCur].ram;
			iSumBandwidth += m_vecRequestBuffer[iCur].bandwidth;
			iSumStorage += m_vecRequestBuffer[iCur].storage;
			if ((iSumCpus <= m_resource[i].cpus) && (iSumRam <= m_resource[i].ram) && (iSumBandwidth <= m_resource[i].bandwidth) && (iSumStorage <= m_resource[i].storage))
			{
				setCurY.insert(m_vecRequestBuffer[iCur].index);
				//setCurYRequext.insert(m_vecRequestBuffer[iCur]);
			}
			else
			{
				iSumCpus -= m_vecRequestBuffer[iCur].cpus;
				iSumRam -= m_vecRequestBuffer[iCur].ram;
				iSumBandwidth -= m_vecRequestBuffer[iCur].bandwidth;
				iSumStorage -= m_vecRequestBuffer[iCur].storage;
				break;
			}
		}
		if (setCurY.size() == iRequestBufferSize)//setCurY�Ѿ���������������е�����
		{
			break;
		}
	}
	//���Դ�m_vecRequestBuffer��ȥ����setCurY�е�����
	for (int i = 0; i < m_vecRequestBuffer.size(); i++)
	{
		if (setCurY.count(m_vecRequestBuffer[i].index) == 0)
		{
			m_vecRequestBufferRemoveTmp.push_back(m_vecRequestBuffer[i]);
		}
	}
	//cout << "In The End of InitFillRequestToCurY:" << iSumCpus << "   " << iSumRam << "   " << iSumBandwidth << "   " << iSumStorage << endl;
	cout << "��ǰ��Դ" << i << "���������������: " << setCurY.size() << endl;
}

//�����㷨����������Բ�����
int ResourceAllocationPolicy::SimulatedAnnealing()
{
	for (int i = 0; i < RESOURCE_COUNT; i++)
	{
		setCurY.clear();//��ǰ���Լ���y�����󼯣���ʼΪ��
		setCurYRequext.clear();
		m_vecRequestBufferRemoveTmp.clear();//��ǰ����ȥ�������Է����y������
		iRequestBufferSize = m_vecRequestBuffer.size();	//������е�������
		iSumCpus = 0, iSumRam = 0, iSumBandwidth = 0, iSumStorage = 0;

		//step1 begin-----------------------------
		InitFillRequestToCurY(i);			//��ʼ�������ǰ��Դy�������
		//step1 end-------------------------------

		cout << "In SA After InitFillRequest -- iSum:" << iSumCpus << "   " << iSumRam << "   " << iSumBandwidth << "   " << iSumStorage << endl;

		//step2 begin-----------------------------
		int iRequestBufferRemoveTmpSize = m_vecRequestBufferRemoveTmp.size();
		if (iRequestBufferRemoveTmpSize > 0 && setCurY.size() > 0)//ȥ����setCurY�е��������������������
		{
			for (int j = TEMPERATURE; j > 0; j--)
			{
				//step 2.1 Ѱ��setCurY�����cost�����index
				set<int>::iterator itMaxCost = setCurY.begin();
				double dMaxCost = CalcCost(*itMaxCost, i);
				for (set<int>::iterator it = setCurY.begin(); it != setCurY.end(); it++)
				{
					if (CalcCost(*it, i) < CalcCost(*itMaxCost, i))
					{
						itMaxCost = it;
					}						
				}
				dMaxCost = CalcCost(*itMaxCost, i);
				
				//step 2.2 ��m_vecRequestBufferRemoveTmpSize�����ѡȡһ�����󣬳������MaxCost������
				int iReplace = RandomNum(iRequestBufferRemoveTmpSize - 1);	//second request
				if (setCurY.count(m_vecRequestBuffer[iReplace].index) == 0)	//iReplace��Ӧ��������set��,���ҵ���������set��
				{				
					//�ж�iReplace����*itMaxCost�᲻�ᳬ����Դ����//ע��itMaxCost��m_vecRequestBuffer�������index,��iReplace��m_vecRequestBufferRemoveTmp�����
					if ((iSumCpus - m_request[*itMaxCost].cpus + m_vecRequestBufferRemoveTmp[iReplace].cpus <= m_resource[i].cpus) &&
						(iSumRam - m_request[*itMaxCost].ram + m_vecRequestBufferRemoveTmp[iReplace].ram <= m_resource[i].ram) &&
						(iSumBandwidth - m_request[*itMaxCost].bandwidth + m_vecRequestBufferRemoveTmp[iReplace].bandwidth <= m_resource[i].bandwidth) &&
						(iSumStorage - m_request[*itMaxCost].storage + m_vecRequestBufferRemoveTmp[iReplace].storage <= m_resource[i].storage))
					{
						//step 2.2.1 �ж�iReplace�Ĵ����Ƿ�С��*itMaxCost��
						if (CalcCost(m_vecRequestBuffer[iReplace].index, i) < dMaxCost)
						{
							setCurY.erase(itMaxCost);
							setCurY.insert(m_vecRequestBuffer[iReplace].index);
						}
						//step 2.2.2 �Ը���PROBABILITY����iReplace
						else if (RandomNum(99) < PROBABILITY)
						{
							setCurY.erase(itMaxCost);
							setCurY.insert(m_vecRequestBuffer[iReplace].index);
						}
					}
					
				}
			}			
		}
		AllocationResourceToRequest(i);			//������Դ��setCurY�е�����
		RecycleResourceFromFinishedRequest(i);	//����m_resource
		UpdateRequestBuffer(i);					//m_vecRequestBuffer��m_vecA[y]
		//step2 end-----------------------------
	}
	return 0;
}

//������Դ��set�е�����
void ResourceAllocationPolicy::AllocationResourceToRequest(int resourceIndex)
{
	cout << "Before Resource Allocation Size:" << m_resource[resourceIndex].cpus << "   " << m_resource[resourceIndex].ram << "   " << m_resource[resourceIndex].bandwidth << "   " << m_resource[resourceIndex].storage << endl;

	for (set<int>::iterator it = setCurY.begin(); it != setCurY.end(); it++)
	{
		m_resource[resourceIndex].cpus -= m_request[*it].cpus;
		m_resource[resourceIndex].ram -= m_request[*it].ram;
		m_resource[resourceIndex].bandwidth -= m_request[*it].bandwidth;
		m_resource[resourceIndex].storage -= m_request[*it].storage;
		m_vecA[resourceIndex].push_back(m_request[*it]);
		if (*it == 67 || *it == 73)
		{
			cout << "Current Request Size:" << m_request[*it].cpus << "   " << m_request[*it].ram << "   " << m_request[*it].bandwidth << "   " << m_request[*it].storage << endl;
		}
		cout << "��Դ" << resourceIndex << "�����������" << *it << endl;
	}
	cout << "After Resource Allocation Size:" << m_resource[resourceIndex].cpus << "   " << m_resource[resourceIndex].ram << "   " << m_resource[resourceIndex].bandwidth << "   " << m_resource[resourceIndex].storage << endl;
	CalcUsage(resourceIndex);
}
//��������Ӧ��ɣ�������Դ
void ResourceAllocationPolicy::RecycleResourceFromFinishedRequest(int resourceIndex)
{
	//����resource��,������ɵ�����
	for (int k = 0; k < m_vecA[resourceIndex].size(); k++)
	{
		if (m_vecA[resourceIndex][k].endTime <= time)//????????????????????????????????????
		{
			m_resource[resourceIndex].cpus += m_vecA[resourceIndex][k].cpus;
			m_resource[resourceIndex].ram += m_vecA[resourceIndex][k].ram;
			m_resource[resourceIndex].bandwidth += m_vecA[resourceIndex][k].bandwidth;
			m_resource[resourceIndex].storage += m_vecA[resourceIndex][k].storage;
			m_vecA[resourceIndex].erase(m_vecA[resourceIndex].begin()+k);//ȥ���������е�
		}			
	}
}

void ResourceAllocationPolicy::UpdateRequestBuffer(int resourceIndex)	//���µ�time�������
{
	m_vecRequestBufferRemoveTmp.clear();
	//���Դ�m_vecRequestBuffer��ȥ����setCurY�е�����
	for (int i = 0; i < m_vecRequestBuffer.size(); i++)
	{
		if (setCurY.count(m_vecRequestBuffer[i].index) == 0)
		{
			m_vecRequestBufferRemoveTmp.push_back(m_vecRequestBuffer[i]);
		}
	}
	m_vecRequestBuffer = m_vecRequestBufferRemoveTmp;
	//����5��request
	for (int j = 0; j < 2; j++)
	{
		m_vecRequestBuffer.push_back(m_request[curInsertRequestIndex++]);
	}
	cout << "The end of UpdateRequestBuffer -> m_vecRequestBuffer Size: " << m_vecRequestBuffer.size() << endl;
}

//�������
double ResourceAllocationPolicy::CalcCost(int x, int y)
{
	double dCost = 0;
	double dSum = (double)m_request[x].cpus / m_initResource[y].cpus
				+ (double)m_request[x].ram / m_initResource[y].ram
				+ (double)m_request[x].bandwidth / m_initResource[y].bandwidth
				+ (double)m_request[x].storage / m_initResource[y].storage;
	dSum = dSum / 4;
	dCost =	fabs((double)m_request[x].cpus / m_initResource[y].cpus - dSum)
		  + fabs((double)m_request[x].ram / m_initResource[y].ram - dSum)
		  + fabs((double)m_request[x].bandwidth / m_initResource[y].bandwidth - dSum)
		  + fabs((double)m_request[x].storage / m_initResource[y].storage - dSum);
	return dCost;
}

//����������
double ResourceAllocationPolicy::CalcUsage(int y)
{
	int iCpus = 0, iRam = 0, iBandwidth = 0, iStorage = 0;
	for (int k = 0; k < m_vecA[y].size(); k++)
	{
		iCpus += m_vecA[y][k].cpus;
		iRam += m_vecA[y][k].ram;
		iBandwidth += m_vecA[y][k].bandwidth;
		iStorage += m_vecA[y][k].storage;
	}
	cout << "CpuUsage= " << (double)iCpus / m_initResource[y].cpus << endl;
	cout << "RamUsage= " << (double)iRam / m_initResource[y].ram << endl;
	cout << "BandwidthUsage= " << (double)iBandwidth / m_initResource[y].bandwidth << endl;
	cout << "StorageUsage= " << (double)iStorage / m_initResource[y].storage << endl;
	return 0.0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	system("mode con:cols=100 lines=1000");
	ResourceAllocationPolicy rap;
	rap.Init();
	while (time < 18)
	{
		cout << "-----------------------TIME: " << time << "-----------------------" << endl;
		rap.SimulatedAnnealing();//SA�㷨����������Բ�������Դ������
		time++;
	}
	system("pause");
	return 0;
}


//��ӡ
void ResourceAllocationPolicy::Print()
{
	for (int i = 0; i < REQUEST_COUNT; i++)
	{
		cout << i << ":\t" << m_request[i].cpus << "\t" << m_request[i].ram << "\t" << m_request[i].bandwidth << "\t"
			<< m_request[i].storage << "\t" << m_request[i].startTime << "\t" << m_request[i].endTime << endl;
	}
	cout << m_vecA[0].size() << endl;
	cout << m_vecA[1].size() << endl;
	cout << m_vecA[2].size() << endl;
	cout << m_vecRequestBuffer.size() << endl;
	for (int j = 0; j < m_vecRequestBuffer.size(); j++)
	{
		cout << m_vecRequestBuffer[j].cpus << "\t";
		cout << m_vecRequestBuffer[j].ram << "\t";
		cout << m_vecRequestBuffer[j].bandwidth << "\t";
		cout << m_vecRequestBuffer[j].storage << "\t\n";
	}
}




void test()
{
	/*vector<int> iVec;
	iVec.push_back(3);
	iVec.push_back(6);
	iVec.push_back(2);
	iVec.push_back(8);
	iVec.push_back(1);
	cout << "before...size: " << iVec.size() << endl;*/
	//iVec.erase(iVec.begin());
	//for (vector<int>::iterator it = iVec.begin(); it != iVec.end();)
	//{
	//	cout << *it << endl;
	//	if (*it == 8)
	//	{
	//		it = iVec.erase(it);
	//	}
	//	else
	//		++it;
	//}
	//cout << "after...size: " << iVec.size() << endl;
	//for (vector<int>::iterator it = iVec.begin(); it != iVec.end();)
	//{
	//	cout << *it << endl;
	//	++it;
	//}

	/*set<int> s;
	s.insert(3);
	s.insert(5);
	s.insert(1);
	set<int>::iterator it = s.find(7);
	if (it == s.end())
	cout << "not found is returning s.end()" << endl;*/
}