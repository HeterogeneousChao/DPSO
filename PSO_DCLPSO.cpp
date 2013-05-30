/*
** D-CLPSO
*/
#include"PSO_DCLPSO.h"

#include<iostream>
using namespace std;

// ----------------------------------------------------------------------------
// ���ü���ʼ��
// ----------------------------------------------------------------------------
void PSO_DCLPSO::SetConfig( int par , int ite , double w , double f , double p1 , double p2 , double p3 )
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
int* PSO_DCLPSO::Evolve()
{
	/*
	ofstream outfile("result.txt",ios::app);
	outfile<<"-------------------------------------"<<endl;
	outfile<<"Evolve"<<endl;
	outfile<<"-------------------------------------"<<endl;
	*/

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

	// PC[i]
	double *Pc = new double[config.population] ;
	for( int k = 0 ; k < config.population ; k++ )
		Pc[k] = 0.05 + 0.45 * ((exp((double)(10*k)/(double)(config.population-1)) - 1) / (exp((double)10) - 1));

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
		int index = 0 ;
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ , index++ )  
		{
			// CL
			// random > Pc[i] , learn from own pbest
			if( (double)(rand()%1000)/1000.0 > Pc[index] )
			{
				i->velocityUpdate_Comprehensive( config.weight , config.factor , pro1_threshold , i->pbest );
				i->positionUpdate( pro2_threshold , pro3_threshold );
			}
			// random < Pc[i] , learn from other pbest
			else
			{
				int f1, f2 ;
				do {
					f1 = (int)(((double)(rand()%1000)/1000.0)*config.population) ;
				} while( f1 == index );
				do {
					f2 = (int)(((double)(rand()%1000)/1000.0)*config.population) ;
				} while( f2 == index );

				vector<DParticle>::const_iterator x1 = T.begin() + f1 ;
				vector<DParticle>::const_iterator x2 = T.begin() + f2 ;

				if( sut->FitnessValue(x1->position,0) > sut->FitnessValue(x2->position,0) )
					i->velocityUpdate_Comprehensive( config.weight , config.factor , pro1_threshold , x1->pbest );
				else
					i->velocityUpdate_Comprehensive( config.weight , config.factor , pro1_threshold , x2->pbest );
				
				i->positionUpdate( pro2_threshold , pro3_threshold );
			}
		} 
		
		// iteration++
		it++ ;

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