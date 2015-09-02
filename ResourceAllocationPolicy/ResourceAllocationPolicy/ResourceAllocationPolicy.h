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
	int cpus;		//个数
	int ram;		//MB
	int bandwidth;	//
	int storage;	//GB
};

struct Request
{
	int index;		//请求index
	int cpus;		//个数
	int ram;		//MB
	int bandwidth;	//
	int storage;	//GB
	int startTime;
	int endTime;
};

class ResourceAllocationPolicy
{
public:
	void Init();								//初始化
	void AllocationResourceToRequest(int resourceIndex);//当前资源分配给其上的请求
	void RecycleResourceFromFinishedRequest(int resourceIndex);	//请求已响应完成，回收资源
	void UpdateRequestBuffer(int resourcIndex);				//更新第time次请求池
	void GenerateRequest();						//动态产生请求
	void Print();								//打印Request[]
	void InitFillRequestToCurY(int i);			//初始对资源y选取随机的请求
	double CalcCost(int x, int y);					//计算代价
	double CalcUsage(int y);							//请算利用率
	int RandomNum(int num);						//产生[0,num]间的随机整数
	int SimulatedAnnealing();					//模拟退火

private:
	Resource m_resource[RESOURCE_COUNT];		//资源(可用资源)
	Resource m_initResource[RESOURCE_COUNT];	//初始资源
	Request m_request[REQUEST_COUNT];			//所有请求数
	vector<Request> m_vecA[RESOURCE_COUNT];		//资源y分配的所有请求，m_vecA[y]
	vector<Request> m_vecRequestBuffer;			//当前请求
	vector<Request> m_vecRequestBufferRemoveTmp;//当前请求去除掉尝试分配给y的请求
	set<Request> setCurYRequext;				//当前尝试加入y的请求
	//int A[RESOURCE_COUNT][REQUEST_COUNT];		//资源y分配是否分配给请求x，A[y][x]
	set<int> setCurY;							//当前尝试加入y的请求	
	int iRequestBufferSize;						//请求池中的请求数
	int iSumCpus = 0, iSumRam = 0, iSumBandwidth = 0, iSumStorage = 0;
};