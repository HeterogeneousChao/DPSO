/*
** Base PSO
*/
#include"PSO.h"

#include<ctime>
#include<cstdlib>
#include<cmath>
#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------
// ����
// ----------------------------------------------------------------------------
void PSO::clear_PSOResult()
{
	for( vector<int*>::iterator i = PSO_Result.begin() ; i != PSO_Result.end() ; i++ )
		delete[] (*i) ;
	PSO_Result.clear();
}

void PSO::copy( vector<int*> A , vector<int*> &B , int prasize )
{
	// �ж�B�Ƿ��ѷ��䣬���ѷ��䣬ɾ��B
	if( B.size() != 0 )
	{
		for( vector<int*>::iterator i = B.begin() ; i != B.end() ; i++ )
			delete[] (*i) ;
		B.clear();
	}
	// �����¿ռ䣬������
	for( vector<int*>::iterator i = A.begin() ; i != A.end() ; i++ )
	{
		int *test = new int[prasize] ;
		for( int v=0 ; v<prasize ; v++ )
			test[v] = (*i)[v] ;
		B.push_back(test);
	}
}


// ----------------------------------------------------------------------------
// Hamming Distance
// ----------------------------------------------------------------------------
double PSO::HammingDist( const int *test )
{
	int hamming = 0 ;
	if( PSO_Result.size() == 0 )
		return 0 ;

	for( vector<int*>::const_iterator i = PSO_Result.begin() ; i != PSO_Result.end() ; i++ )
	{
		int ham = 0 ;
		for( int k=0 ; k<sut->parameter ; k++ )
		{
			if( test[k] != (*i)[k] )
				ham++ ;
		}
		hamming += ham ;
	}
	double h = (double)hamming / (double)PSO_Result.size() ;
	return h ;
}


// ----------------------------------------------------------------------------
// One-test-at-a-time
// ----------------------------------------------------------------------------
void PSO::PSOEvolve()
{
	PSO_SIZE = MAX ;
	PSO_TIME = MAX ;
	//PSO_COV = 0 ;

	// ���
	clear_PSOResult(); 
	//FEtimes = 0 ;

	//
	// ��ʼִ��
	//

	// ��ʼ��
	sut->GenerateS();
	time_t nowtime1 = time( 0 );

	// ��һ���ɲ�������
	while( sut->SCount != 0 )
	{
		int *best = Evolve();
		PSO_Result.push_back( best );
		int cov = sut->FitnessValue( best , 1 );

		cout<<PSO_Result.size()<<", "<<sut->SCountAll-sut->SCount<<endl;

		//if( FEtimes > 10000 )
		//	break;
	}

	time_t nowtime2 = time(0);
	
	PSO_SIZE = PSO_Result.size();
	PSO_TIME = nowtime2 - nowtime1 ;
	//PSO_COV = sut->SCountAll - sut->SCount ;

	// ��������ӣ�������TResult
	if( sut->seedInfo.size() != 0 )
	{
		vector<int*> temp ;
		copy( PSO_Result , temp , sut->parameter );
		clear_PSOResult(); 
		copy( sut->seedInfo , PSO_Result , sut->parameter );
		for( vector<int*>::iterator i = temp.begin() ; i != temp.end() ; i++ )
			PSO_Result.push_back((*i)) ;
		PSO_SIZE = PSO_SIZE + sut->seedInfo.size() ;
	}
}