#include <vector>
using namespace std;
#define RESOURCE_COUNT 3
#define REQUEST_COUNT 100
#define TEMPERATURE 3
#define PROBABILITY	10 
int time = 1;
int curInsertRequestIndex = 10;

struct Resource
{
	int cpus;		//����
	int ram;		//MB
	int bandwidth;	//
	int storage;	//GB
};

struct Request
{
	int index;		//����index
	int cpus;		//����
	int ram;		//MB
	int bandwidth;	//
	int storage;	//GB
	int startTime;
	int endTime;
};

class ResourceAllocationPolicy
{
public:
	void Init();								//��ʼ��
	void AllocationResourceToRequest(int resourceIndex);//��ǰ��Դ��������ϵ�����
	void RecycleResourceFromFinishedRequest(int resourceIndex);	//��������Ӧ��ɣ�������Դ
	void UpdateRequestBuffer(int resourcIndex);				//���µ�time�������
	void GenerateRequest();						//��̬��������
	void Print();								//��ӡRequest[]
	void InitFillRequestToCurY(int i);			//��ʼ����Դyѡȡ���������
	double CalcCost(int x, int y);					//�������
	double CalcUsage(int y);							//����������
	int RandomNum(int num);						//����[0,num]����������
	int SimulatedAnnealing();					//ģ���˻�

private:
	Resource m_resource[RESOURCE_COUNT];		//��Դ(������Դ)
	Resource m_initResource[RESOURCE_COUNT];	//��ʼ��Դ
	Request m_request[REQUEST_COUNT];			//����������
	vector<Request> m_vecA[RESOURCE_COUNT];		//��Դy�������������m_vecA[y]
	vector<Request> m_vecRequestBuffer;			//��ǰ����
	vector<Request> m_vecRequestBufferRemoveTmp;//��ǰ����ȥ�������Է����y������
	set<Request> setCurYRequext;				//��ǰ���Լ���y������
	//int A[RESOURCE_COUNT][REQUEST_COUNT];		//��Դy�����Ƿ���������x��A[y][x]
	set<int> setCurY;							//��ǰ���Լ���y������	
	int iRequestBufferSize;						//������е�������
	int iSumCpus = 0, iSumRam = 0, iSumBandwidth = 0, iSumStorage = 0;
};