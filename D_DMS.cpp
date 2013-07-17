/*
** Discrete DMS-PSO
*/
#include"D_DMSPSO.h"

#include<iostream>
#include"Windows.h"
using namespace std;

// ----------------------------------------------------------------------------
// ���ü���ʼ��
// ----------------------------------------------------------------------------
void D_DMSPSO::SetConfig( int par , int ite , double w , double f , double p1 , double p2 , double p3 )
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
int* D_DMSPSO::Evolve()
{
	double inertia = 0.9 ;
	double inertia_max = 0.9 ;
	double inertia_min = 0.4 ;

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

	// DMS
	int M = 4 ; // ÿ����Ⱥ������
	int R = 5 ;
	int SS = config.population/M ;  // ��Ⱥ��
	int *Swarms = new int[config.population] ;
	int *Swarms_num = new int[SS] ;

	int **Swarms_localpos = new int*[SS] ;
	for( int k=0; k<SS; k++ )
		Swarms_localpos[k] = new int[sut->parameter] ;

	int *Swarms_localbest = new int[SS] ;
	for( int k=0; k<SS; k++ )
		Swarms_localbest[k] = -1 ;

	// random divide population
	for( int k=0 ; k<SS ; k++ )
		Swarms_num[k] = 0 ;

	for( int k=0 ; k<config.population; k++ )
	{
		int a = (int)( (double)(rand()%1000)/1000.0 * SS );
		while( Swarms_num[a] == M )
			a = (int)( (double)(rand()%1000)/1000.0 * SS );

		Swarms[k] = a ;
		Swarms_num[a]++ ;
	}
	/*
	cout<<"Swarms"<<endl;
	for( int c=0 ; c<config.population ; c++ )
		cout<<Swarms[c]<<" ";
	cout<<endl<<endl;
	cout<<"Swarms_num"<<endl;
	for( int k=0 ; k<SS ; k++ )
		cout<<Swarms_num[k]<<" ";
	cout<<endl<<endl;
	cout<<"Swarms_localbest"<<endl;
	for( int k=0 ; k<SS ; k++ )
		cout<<Swarms_localbest[k]<<" ";
	cout<<endl<<endl;
	cout<<"Swarms_localpos"<<endl;
	for( int k=0; k<SS; k++ )
	{
		for( int c=0 ; c<sut->parameter ; c++ )
			cout<<Swarms_localpos[k][c]<<" ";
		cout<<endl;
	}
	*/
	//Sleep(100000000000);

	// ����һ������������gBest
	while( true )
	{
		// ����ÿ�����ӵ�fitnessֵ��������pbest��gbest
		int index = 0 ;
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ , index++ )
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

			// ���� lbest
			for( int k=0 ; k<config.population; k++ )
			{
				if( Swarms[k] == Swarms[index] && fit > Swarms_localbest[Swarms[k]] )
				{
					Swarms_localbest[Swarms[k]] = fit ;
					for( int c = 0 ; c< sut->parameter ; c++)
						Swarms_localpos[Swarms[k]][c] = (*i).position[c] ;
				}
			}

		}  // end for 

		// ��ֹ����
		if ( it >= (int)((double)config.iteration * 0.9) )
			break ;

		// ��������Ⱥ
		index = 0 ;
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++ , index++ )  
		{
			i->velocityUpdate( inertia, config.factor, config.factor, pro1_threshold , Swarms_localpos[Swarms[index]] );
			i->positionUpdate( pro2_threshold , pro3_threshold );
		}

		// iteration++
		it++ ;

		// inertia 0.9->0.4
		inertia = inertia_max - ( inertia_max - inertia_min ) * ( (double)it / (double)config.iteration );


		// re divided swarm
		if( it % R == 0 )
		{
			// random divide population
			for( int k=0 ; k<SS ; k++ )
				Swarms_num[k] = 0 ;

			for( int k=0 ; k<config.population; k++ )
			{
				int a = (int)( (double)(rand()%1000)/1000.0 * SS );
				while( Swarms_num[a] == M )
					a = (int)( (double)(rand()%1000)/1000.0 * SS );
				Swarms[k] = a ;
				Swarms_num[a]++ ;
			}

			for( int k=0; k<SS; k++ )
				Swarms_localbest[k] = 0 ;
		}
		

	}  // end 0.9 while


	// last 0.1
	while( true )
	{
		// ����ÿ�����ӵ�fitnessֵ��������pbest��gbest
		for( vector<DParticle>::iterator i = T.begin() ; i != T.end() ; i++  )
		{
			int fit = sut->FitnessValue( (*i).position , 0 ) ;

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
			i->velocityUpdate( inertia, config.factor, config.factor, pro1_threshold , gBest );
			i->positionUpdate( pro2_threshold , pro3_threshold );
		} 

		// iteration++
		it++ ;

		// inertia 0.9->0.4
		inertia = inertia_max - ( inertia_max - inertia_min ) * ( (double)it / (double)config.iteration );

	}  // end 0.1 while

	for( int k = 0 ; k < sut->parameter ; k++ )   // best = gBest.position
		best[k] = gBest[k] ;

	//for( int k = 0 ; k < sut->parameter ; k++ )
	//	cout<<gBest[k]<<" " ;
	//cout<<endl;

	delete[] gBest ;
	for( vector<DParticle>::iterator j = T.begin() ; j != T.end() ; j++ )
		j->clear();
	T.clear();

	delete[] Swarms ;
	delete[] Swarms_num ;

	for( int k=0; k<SS; k++ )
		delete[] Swarms_localpos[k] ;
	delete[] Swarms_localpos ;

	return best ;
}