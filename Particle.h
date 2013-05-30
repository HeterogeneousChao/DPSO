/*
** Conventional Particle
*/
#pragma once

#include<cstdlib>
#include<cmath>
using namespace std;

class Particle
{
	// ���캯��
public:
	Particle( int dim , int* ran )
	{
		dimension = dim ;
		range = ran ;

		position = new int[dimension] ;
		velocity = new double[dimension] ;
		pbest = new int[dimension] ;
	};
	~Particle()
	{
	}

public:
	// ����������ȡֵ��ָ��
	int dimension ;
	int* range ;

	// ��ǰλ�ã��ٶȼ���Ӧֵ
	int* position ;
	double* velocity ;
	int fitness_now ;

	// ��ʷ������λ�ü���Ӧֵ
	int* pbest ;
	int fitness_pbest ;

public:
	void RandomInit() ;  // �����ʼ��

	// �õ�ǰλ�ø�����ʷ����
	void Setpbest( int fit );

	// ����
	void velocityUpdate( double weight , double factor , const int* gbest ) ;
	void positionUpdate() ;

	// CLPSO����
	void velocityUpdateDim( double weight, double factor , const int* pbest, int dim );

	void clear();

};







