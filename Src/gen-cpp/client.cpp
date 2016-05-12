#include "ProxyServer.h"
#include <fstream>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <sys/time.h>
#include <random>

#define TS(x) ((x.tv_sec*1000000)+(x.tv_usec))

using namespace std;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

std::fstream& GotoLine(std::fstream& file, unsigned int num){
	file.seekg(std::ios::beg);
	for(int i=0; i < num - 1; ++i){
		file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	}
	return file;
}


int main(int argc,char **argv){
	if(argc!=4){
		printf("%s <mode> <server addr> <distribution policy/url to fetch>\n",argv[0]);
		return -1;
	}
	using namespace std;
	boost::shared_ptr<TTransport> socket(new TSocket(argv[2], 9093));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	struct timeval start,end;
	unsigned long long int total_time=0;
	ProxyServerClient client(protocol);
	transport->open();
	string pageData;

	int mode = atoi(argv[1]);
	//Only fecth one url
	if(mode == 0){
		client.fetchURL(pageData,argv[3]);
		std::cout<<pageData<<endl;
	}
	//Test the client
	else{

		fstream file("url.txt");
		if(atoi(argv[3]) == 0){
			std::cout<<"Uniform Random Distribution\n";
			srand (123456789);
			for (int i = 0; i < 100; ++i)
			{
				int rand_index = (rand() % 20) +1;
				GotoLine(file, rand_index);
				string line;
				file >> line;
				gettimeofday(&start, NULL);
				std::cout<<line<<endl;
				client.fetchURL(pageData,line);
				gettimeofday(&end, NULL);
				total_time += TS(end)-TS(start);
			}
		}
		else
		{
			std::cout<<"Exponential Distribution\n";
			std::default_random_engine generator;
			std::exponential_distribution<double> distribution(3.5);
			const int nintervals = 20;
			int url[20] = {};
			for (int i=0; i<100; ++i) {
				double number = distribution(generator);
				if (number<1.0) {
					url[int(nintervals*number)]++;
					GotoLine(file, int(nintervals*number)+1);
					string line;
					file >> line;
					gettimeofday(&start, NULL);
					std::cout<<line<<endl;
					client.fetchURL(pageData,line);
					gettimeofday(&end, NULL);
					total_time += TS(end)-TS(start);
				}
			}
		} 

		printf("Time to complete workload: %3.2f sec\n",total_time/float(1000000));
	}
	transport->close();

	return 0;
}
