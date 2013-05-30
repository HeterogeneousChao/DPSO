#include"Particle.h"

#include<iostream>
using namespace std;

void Particle::clear()
{
	delete[] position ;
	delete[] velocity ;
	delete[] pbest ;
}

// ----------------------------------------------------------------------------
// �����ʼ����fitness��ʼΪ0
// dim = ά�ȣ�range = ÿ��ά��ȡֵ
// ----------------------------------------------------------------------------
void Particle::RandomInit()
{
	for( int i = 0 ; i < dimension ; i++ )
	{
		position[i] = (int)( ((double)(rand()%1000)/1000.0) * range[i] );
		velocity[i] = 2 * ((double)(rand()%1000)/1000.0) - 1  ;
	}
	fitness_now = 0 ;

	for( int i = 0 ; i < dimension ; i++ )
		pbest[i] = position[i] ;
	fitness_pbest = 0 ;
}

// ----------------------------------------------------------------------------
// ������ʷ����
// ----------------------------------------------------------------------------
void Particle::Setpbest( int fit )
{
	for( int i = 0 ; i < dimension ; i++ )
		pbest[i] = position[i] ;
	fitness_pbest = fit ;
}

// ----------------------------------------------------------------------------
// velocity
// ������weight , factor
// ----------------------------------------------------------------------------
void Particle::velocityUpdate( double weight , double factor , const int* gbest )
{
	for ( int v = 0 ; v < dimension ; v++ )
	{
		double r1 = (double)(rand()%1000)/1000.0 ;
		double r2 = (double)(rand()%1000)/1000.0 ;

		velocity[v] = (double) weight * velocity[v]  +
			(double) factor * r1 * ( pbest[v] - position[v] ) +
			(double) factor * r2 * ( gbest[v] - position[v] ) ;

		// ����ٶȴ���
		if( velocity[v] > range[v]/2 )
			velocity[v] = (double)range[v]/2  ;
		if( velocity[v] < -range[v]/2 )
			velocity[v] = (double)-range[v]/2 ;	

	}
}

// ----------------------------------------------------------------------------
// velocity dim
// ----------------------------------------------------------------------------
void Particle::velocityUpdateDim( double weight, double factor , const int* pbest, int dim )
{
	double r1 = (double)(rand()%1000)/1000.0 ;
	
	velocity[dim] = (double) weight * velocity[dim]  +
			(double) factor * r1 * ( pbest[dim] - position[dim] );

	// ����ٶȴ���
	if( velocity[dim] > range[dim]/2 )
		velocity[dim] = (double)range[dim]/2  ;
	if( velocity[dim] < -range[dim]/2 )
		velocity[dim] = (double)-range[dim]/2 ;	
			
}


// ----------------------------------------------------------------------------
// position
// ������version 0 - Absorbing Walls
//               1 - Reflecting Walls
//               2 - Cyclic Walls
// ----------------------------------------------------------------------------
void Particle::positionUpdate()
{
	for ( int v = 0 ; v < dimension ; v++ )
	{
		position[v] = position[v] + (int)velocity[v] ;

		// �߽紦��
		/*
		if( version == 0 )   // Absorbing Walls
		{
			if( position[v] >= range[v] )
				position[v] = range[v]-1 ;
			if ( position[v] < 0 )
				position[v] = 0 ;
		}
		*/
		//if( version == 1 )   // Reflecting Walls
		//{
			if( position[v] >= range[v] )
				position[v] = 2*range[v]-position[v]-1 ;
			if ( position[v] < 0 )
				position[v] = -position[v]-1 ;
		//}
		/*
		if( version == 2 )  // Cyclic Walls
		{
			if( position[v] >= range[v] )
				position[v] = position[v]-range[v] ;
			if ( position[v] < 0 )
				position[v] = position[v]+range[v] ;
		}*/
	}
}