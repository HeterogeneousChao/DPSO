/*
** Discrete Particle
*/
#pragma once

#include<cstdlib>
#include<cmath>
#include<map>
#include<set>
#include<iostream>
using namespace std;

struct PosVal
{
	int posI ;    // ����������
	int valI ;    // ȡֵ������

	bool operator < (PosVal const& _PV) const
	{
		if( posI < _PV.posI || ( posI == _PV.posI && valI < _PV.valI ) )
			return true ;
		else
			return false ;
	}
};

class DParticle
{
	// ���캯��
public:
	DParticle( int dim , int* ran , int s )
	{
		dimension = dim ;
		range = ran ;
		
		// cal
		strength = s ;
		PosMax = cal_combine( dim , s );
		ValMax = new int[PosMax] ;
		for( int k=0 ; k<PosMax ; k++ )
		{
			int comb = 1 ;
			int *pos = cal_num2pos( k );
			for( int p=0 ; p<s ; p++ )
				comb = comb * range[pos[p]] ;
			delete[] pos ;
			ValMax[k] = comb ;
		}

		//position = NULL ;
		//pbest = NULL ;
		position = new int[dimension] ;
		pbest= new int[dimension] ;
	};
	~DParticle()
	{
		
	}

public:
	// ����������ȡֵ��ָ��
	int dimension ;
	int* range ;

	int strength ;    // ����ǿ��
	int PosMax ;    // �����������
	int *ValMax ;  // ��ȡֵ�������

	// ��ǰλ�ã��ٶȼ���Ӧֵ
	int* position ;
	map<PosVal,double> velocity ;
	int fitness_now ;

	// ��ʷ������λ�ü���Ӧֵ
	int* pbest ;
	int fitness_pbest ;

	// ��ʱ
	set<pair<double,PosVal>> TempV ;

public:
	void RandomInit() ;  // �����ʼ��

	// �õ�ǰλ�ø�����ʷ����
	void Setpbest( int fit );

	// ����
	void velocityUpdate( double weight , double factor1 , double factor2, double prob1 , const int* gbest ) ;
	void positionUpdate( double prob2 , double prob3 ) ;

	// ����CLPSO
	void velocityUpdate_Comprehensive( double weight , double factor, double prob1, const int* clbest );

	void clear();

public:
	int cal_combine( const int n , const int m );   // C(n,m)
	int cal_pos2num( const int *pos );  // ��C(n,m)�У����ݾ���λ��pos�������
	int* cal_num2pos( int t );  // ��C(n,m)�У�������ŷ��ؾ���λ��pos

	int cal_val2num( const int *val , const int *pos );   // ��posλ�ã�����val�������  
	int* cal_num2val( int t , const int *pos );     // ��posλ�ã�������ŷ���val

};







