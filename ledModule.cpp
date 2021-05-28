#include "systemc.h"
#include <tce_systemc.hh>


TCE_SC_OPERATION_SIMULATOR(RegS_In) {
sc_signal<unsigned int> indX,indY;
sc_signal<unsigned int> Centeral;
sc_in<unsigned int> SouthEast,South,SouthWest,West,NorthWest,North,NorthEast,East;
sc_out<unsigned int> vitrin;
unsigned int Readed_value_out;

TCE_SC_OPERATION_SIMULATOR_CTOR(RegS_In) {}
TCE_SC_SIMULATE_CYCLE_START {}

	TCE_SC_SIMULATE_STAGE {
	

	if (TCE_SC_OPNAME.compare("READ_NEIGHNOUR")==0){
			unsigned int Neighbor_Index = TCE_SC_UINT(1);

			if (Neighbor_Index == 0)  
				 Readed_value_out=Centeral.read();
			else if (Neighbor_Index == 1)
				Readed_value_out=SouthEast.read();
			else if (Neighbor_Index == 2)
				Readed_value_out=South.read();
			else if (Neighbor_Index == 3)
				Readed_value_out=SouthWest.read();
			else if (Neighbor_Index == 4)
				Readed_value_out=West.read();
			else if (Neighbor_Index == 5)
				Readed_value_out= NorthWest.read();	
			else if (Neighbor_Index == 6)
				Readed_value_out=North.read();
			else if (Neighbor_Index == 7)
				Readed_value_out=NorthEast.read();
			else if (Neighbor_Index == 8)
				Readed_value_out=East.read();
			else
				Readed_value_out = 999;

			TCE_SC_OUTPUT(2) = Readed_value_out;
			}

		if (TCE_SC_OPNAME.compare("READ_INDEX") == 0 ){
			unsigned int IND = TCE_SC_UINT(1);

			if (IND == 0)		  // X index
				TCE_SC_OUTPUT(2)=indX.read();
			else if (IND == 1)        // Y index
				TCE_SC_OUTPUT(2)=indY.read();
			}

		if (TCE_SC_OPNAME.compare("WRITE_VITRIN") == 0 ){
				vitrin.write(TCE_SC_UINT(1)); 
			}

	return true;
}

};



////Other Moduels (SystemC only)

SC_MODULE(Display_8_bit)
{
 sc_in<unsigned char> A ;
 sc_in<bool> Clk;
 void StimGen()
 {
 cout << (unsigned int) A.read() << "  ";
 }
 SC_CTOR(Display_8_bit)
 {
 SC_METHOD(StimGen);
//  sensitive << A; Only Prints when the value changes
sensitive << Clk.pos() ; // Print the value once each clock cycle
 }
};


TCE_SC_OPERATION_SIMULATOR(LED_Moduel) {
//sc_in<unsigned char> reg_value_in;
sc_out<unsigned int> reg_value_out;
TCE_SC_OPERATION_SIMULATOR_CTOR(LED_Moduel) { }
TCE_SC_SIMULATE_CYCLE_START { }
	TCE_SC_SIMULATE_STAGE {
	unsigned address = TCE_SC_UINT(1);
	reg_value_out.write(TCE_SC_UINT(1));  // Output to be used by other modules 
//	TCE_SC_OUTPUT(2) = TCE_SC_UINT(1);// Core Dump problem got solved after commenting this line, THIS MODULE DID NOT HAVE A OUTPUT PORT!!!
	return true;
}
};

