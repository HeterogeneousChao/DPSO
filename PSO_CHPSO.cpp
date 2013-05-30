/*
** Conventional TVAC
*/
#include"PSO_CHPSO.h"

// ----------------------------------------------------------------------------
// ���ü���ʼ��
// ----------------------------------------------------------------------------
void PSO_CHPSO::SetConfig( int par , int ite )
{
	config.population = par ;
	config.iteration = ite ;
}

// ----------------------------------------------------------------------------
// ����һ����������
// ----------------------------------------------------------------------------
int* PSO_CHPSO::Evolve()
{
	double inertia = 0.9 ;
	double inertia_max = 0.9 ;
	double inertia_min = 0.4 ;
	double factor1 = 1.3 ;
	double factor2 = 1.3 ;
	double factor_max = 1.8 ;
	double factor_min = 0.8 ;

	// ����ֵ
	int *best = new int[sut->parameter] ;  
	
	vector<Particle> T ; 	               // ����Ⱥ
	int *gBest = new int[sut->parameter];  // ��Ⱥ����
	int fitbest = 0 ;

	// ��ʼ������Ⱥ
	for( int i = 0 ; i < config.population ; i++ )
	{
		Particle a( sut->parameter , sut->value ) ;
		a.RandomInit();

		T.push_back(a);
	}

	// gBest = T[0]
	vector<Particle>::iterator x = T.begin();     
	for( int c = 0 ; c < sut->parameter ; c++)
		gBest[c] = (*x).position[c] ;

	// ��������
	int it = 1 ;

	// ����һ������������gBest
	while( true )
	{
		// ����ÿ�����ӵ�fitnessֵ��������pbest��gbest
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

			// ��fitness(t) = coverMax �� ����
			if( fit == sut->testcaseCoverMax )   	
			{
				for( int c = 0 ; c< sut->parameter ; c++)
					best[c] = (*i).position[c] ;

				delete[] gBest ;
				for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
					i->clear();
				T.clear();

				return best ;
			}

			// ����pBest
			if ( fit > (*i).fitness_pbest )
				(*i).Setpbest( fit );
			
			// ����gBest
			if ( fit > fitbest )    
			{
				fitbest = fit ;
				for( int c = 0 ; c < sut->parameter ; c++)
					gBest[c] = (*i).position[c] ;
			}
		}  // end for

		// ��ֹ����
		if ( it >= config.iteration )
			break ;

		// ��������Ⱥ
		for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )  
		{
			(*i).velocityUpdate( config.weight , config.factor , gBest );
			(*i).positionUpdate();
		} 

		// iteration++
		it++ ;

		// TVAC
		inertia = inertia_max - ( inertia_max - inertia_min ) * ( (double)it / (double)config.iteration );
		factor1 = ( factor_min - factor_max ) * ( (double)it / (double)config.iteration ) + factor_max ;
		factor2 = ( factor_max - factor_min ) * ( (double)it / (double)config.iteration ) + factor_min ;

	}  // end while

	for( int k = 0 ; k < sut->parameter ; k++ )   // best = gBest.position
		best[k] = gBest[k] ;

	delete[] gBest ;
	for( vector<Particle>::iterator i = T.begin() ; i != T.end() ; i++ )
		i->clear();
	T.clear();

	return best ;
}