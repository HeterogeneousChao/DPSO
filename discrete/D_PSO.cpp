/*
** Discrete PSO
*/
#include "D_PSO.h"

// ----------------------------------------------------------------------------
// ���ü���ʼ��
// ----------------------------------------------------------------------------
void D_PSO::SetConfig( int par , int ite , double w , double f , double p1 , double p2 , double p3 )
{
	config.population = par ;
	config.iteration = ite ;
	config.weight = w ;
	config.factor = f ;

	pro1_threshold = p1 ;
	pro2_threshold = p2 ;
	pro3_threshold = p3 ;
}


// ----------------------------------------------------------------------------
// ����һ����������
// ----------------------------------------------------------------------------
int* D_PSO::Evolve()
{

	// ����ֵ
	int *best = new int[sut->parameter] ;  
	
	vector<DParticle> T ; 	               // ����Ⱥ

	// ��Ⱥ����λ�ü���Ӧֵ
	int *gBest = new int[sut->parameter];  
	int fitbest = 0 ;

	// ��ʼ������Ⱥ
	for( int i = 0 ; i < config.population ; i++ )
	{
		DParticle a( sut->parameter , sut->value , sut->tway ) ;
		a.RandomInit();

		T.push_back(a);
	}

	// gBest = T[0]
	vector<DParticle>::iterator x = T.begin();     
	for( int c = 0 ; c < sut->parameter ; c++)
		gBest[c] = (*x).position[c] ;

	// ��������
	int it = 1 ;

	// inertia 0.9 -> 0.4
	bool adap_inertia = false ;
	if ( config.weight == -1 )
	{
		adap_inertia = true ;
		config.weight = 0.9 ;
	}

	// ����һ������������gBest
	while( true )
	{
		// ����ÿ�����ӵ�fitnessֵ��������pbest��gbest
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

			// ��fitness(t) = coverMax
			//if( fit == sut->testcaseCoverMax )
			if( fit == sut->testcaseCoverMax && PSO_Result.size() == 0 )
			{
				for( int c = 0 ; c< sut->parameter ; c++)
					best[c] = (*i).position[c] ;

				delete[] gBest ;
				for( vector<DParticle>::iterator j = T.begin() ; j != T.end() ; j++ )
					j->clear();
				T.clear();

				return best ;
			}

			// ����pBest
			if ( fit > i->fitness_pbest )
				i->Setpbest( fit );
			
			// ����gBest
			if ( fit > fitbest )    
			{
				fitbest = fit ;
				for( int c = 0 ; c < sut->parameter ; c++)
					gBest[c] = (*i).position[c] ;
			}
			
			else if( fit == fitbest )
			{
				if( HammingDist((*i).position) < HammingDist(gBest) )
				{
					for( int c = 0 ; c< sut->parameter ; c++)
						gBest[c] = (*i).position[c] ;
				}
			}
		}  // end for

		// ��ֹ����
		if ( it >= config.iteration )
			break ;

		// ��������Ⱥ
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ )  
		{
			i->velocityUpdate( config.weight , config.factor , config.factor, pro1_threshold , gBest );
			i->positionUpdate( pro2_threshold , pro3_threshold );
		} 
		
		// iteration++
		it++ ;

		// inertia 0.9 -> 0.4
		if( adap_inertia )
			config.weight = 0.9 - 0.5 * ( (double)it / (double)config.iteration );

	}  // end while

	for( int k = 0 ; k < sut->parameter ; k++ )   // best = gBest.position
		best[k] = gBest[k] ;

	//for( int k = 0 ; k < sut->parameter ; k++ )
	//	cout<<gBest[k]<<" " ;
	//cout<<endl;

	delete[] gBest ;
	for( vector<DParticle>::iterator j = T.begin() ; j != T.end() ; j++ )
		j->clear();
	T.clear();

	return best ;
}