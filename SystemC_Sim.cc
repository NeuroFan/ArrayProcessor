#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <systemc>
#include "ledModule.cpp"
#include "MyTCEVersion.hh"
#include <vector>
#include <iomanip>
#include <stdlib.h>

using namespace std; 

//#include "register.hh"
//#include "lsu_model.hh"

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str();
    }
}	

SC_MODULE(Core)
{
 // sc_signal<unsigned int> South,SouthWest,West,NorthWest,North,NorthEast,East,SouthEast;
 // sc_signal<unsigned int> Vitrin;

  TTACore TTA;
  RegS_In FU_Neignours;

  SC_CTOR(Core) : TTA("processor","processor.adf","asm1.tpef"), FU_Neignours("N")
 // SC_CTOR(Core) : TTA("processor","processor.adf","asm1.tpef"), FU_Neignours("N")
  {
 
    TTA.setOperationSimulator("FU", FU_Neignours);
  }
};




int sc_main(int argc, char* argv[]) {



	
	// 100MHz clock frequency (1 us clock period)
	sc_clock clk("clock", 1, SC_US);
	sc_signal<bool> glock;
	const int M = 10, N = 10;
	sc_signal<unsigned int> Grounded_Wire[M][N];
	sc_signal<unsigned int > wire[M][N]; //This is a wire bundle!
	for (int i=0;i<M;i++)
	   for (int j=0;j<N;j++)
		Grounded_Wire[i][j].write(0);

std::cout<<" Input Array as image: "<<std::endl;



	std::vector<std::vector<Core*> > TTAs;

	for (int i=0 ; i<M; i++){
		TTAs.push_back(std::vector<Core*>());
		for (int j=0;j<N;j++){
		TTAs[i].push_back(new Core(""));
		}
	}
int C=0;
	int i,j;
	for ( i=0; i<M; i++){
		for ( j=0;j<N;j++){
		TTAs[i][j]->TTA.clock(clk);
		TTAs[i][j]->TTA.global_lock(glock);
	        TTAs[i][j]->FU_Neignours.vitrin(wire[i][j]);
	        TTAs[i][j]->FU_Neignours.indX.write(i); //setting the X index
	        TTAs[i][j]->FU_Neignours.indY.write(j); //setting the Y index
		int some_pixel_value = (std::rand()%5)+0;
	        TTAs[i][j]->FU_Neignours.Centeral.write(some_pixel_value); //setting some value as pixel corresponding to the PE index

		std::cout << std::setw(4);
		std::cout<< some_pixel_value;
	
		}
		std::cout << std::setw(5);
		std::cout<<std::endl;
	}

	const int edge_i = M-1, edge_j = N-1;
	for ( i=0; i<M; i++){
		for ( j=0;j<N;j++){
		if (i != edge_i)
		TTAs[i][j]->FU_Neignours.South(wire[i+1][j]);
		else
		TTAs[i][j]->FU_Neignours.South(Grounded_Wire[i][j]);

		if ((i !=edge_i) && (j != 0))
		TTAs[i][j]->FU_Neignours.SouthWest(wire[i+1][j-1]);
		else
		TTAs[i][j]->FU_Neignours.SouthWest(Grounded_Wire[i][j]);

		if (j!=0)
		TTAs[i][j]->FU_Neignours.West(wire[i][j-1]);
		else
		TTAs[i][j]->FU_Neignours.West(Grounded_Wire[i][j]);

		if ((i!=0)&&(j!=0))
		TTAs[i][j]->FU_Neignours.NorthWest(wire[i-1][j-1]);
		else
		TTAs[i][j]->FU_Neignours.NorthWest(Grounded_Wire[i][j]);

		if (i!=0)
		TTAs[i][j]->FU_Neignours.North(wire[i-1][j]);
		else
		TTAs[i][j]->FU_Neignours.North(Grounded_Wire[i][j]);

		if ((i!=0) && (j!=edge_j))
		TTAs[i][j]->FU_Neignours.NorthEast(wire[i-1][j+1]);
		else 
		TTAs[i][j]->FU_Neignours.NorthEast(Grounded_Wire[i][j]);

		if ((j!=edge_j))
		TTAs[i][j]->FU_Neignours.East(wire[i][j+1]);
		else
		TTAs[i][j]->FU_Neignours.East(Grounded_Wire[i][j]);

		if ((i !=edge_i) && (j!=edge_j))
		TTAs[i][j]->FU_Neignours.SouthEast(wire[i+1][j+1]);
		else
		TTAs[i][j]->FU_Neignours.SouthEast(Grounded_Wire[i][j]);
		}

	}
	sc_time runtime(.1, SC_SEC); 
	std::cout<<"Simulation starts";	
	sc_start(runtime);
	std::cout<<"Simulation ends";	
std::cout<<std::endl;



	for (int i=0; i<M;i++){
		for (int j=0;j<N;j++)
			{
			std::cout << std::setw(4);
			std::cout<< wire[i][j].read();
			}
		std::cout << std::setw(5);
		std::cout<<std::endl;
		}


	std::cout<<std::endl;
	std::cout<< "Cycle Count: "<<TTAs[1][1]->TTA.instructionCycles() <<std::endl;

	return EXIT_SUCCESS;
}
