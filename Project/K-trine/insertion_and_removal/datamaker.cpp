#include<bits/stdc++.h>
using namespace std;
vector<unordered_set<int > > Adj;
vector<pair<int ,int > > edg;
int main(int argc, char** argv)
{
	srand(time(0));
	const string op = argv[1];
	const int N = atoi(argv[2]);
	const int M = atoi(argv[3]);
	const int K = atoi(argv[4]);
	FILE* gedges_file;
	FILE* another_file;
	int tarNum=0;
	if(op == "insert"){
		tarNum=M+K;
		gedges_file = fopen("testinc/gedges","w");
		another_file = fopen("testinc/incedges","w");
	}
	else{
		tarNum=M;
		gedges_file = fopen("testdec/gedges","w");
		another_file = fopen("testdec/decedges","w");
	}

	fprintf(gedges_file, "%d %d\n", N, M);
	fprintf(another_file,"%d\n",K);

	int cnt=0;
	Adj.resize(N+1);
	while(cnt<tarNum){
		int u,v;
		u=(rand()*1000ll+rand())%N+1;
		v=(rand()*1000ll+rand())%N+1;
		if(u==v)	continue;
		if(u>v)	swap(u,v);
		if(Adj[u].count(v))	continue;
		cnt++;
		edg.push_back(make_pair(u,v));
		Adj[u].insert(v);
	}
	for(int i=0;i<M;i++)
		fprintf(gedges_file, "%d %d\n", edg[i].first, edg[i].second);
	if(op == "insert"){
		for(int i=M;i<cnt;i++)
			fprintf(another_file, "%d %d\n", edg[i].first, edg[i].second);
	}
	else{
		for(int i=0;i<K;i++)
			fprintf(another_file, "%d %d\n", edg[i].first, edg[i].second);
	}
	fclose(gedges_file),fclose(another_file);
}