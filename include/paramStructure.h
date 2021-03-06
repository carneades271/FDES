
/*==================================================================

Copyright (C) 2015 Wouter Van den Broek, Xiaoming Jiang

This file is part of FDES.

FDES is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FDES is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FDES. If not, see <http://www.gnu.org/licenses/>.

Email: wouter.vandenbroek@uni-ulm.de, wouter.vandenbroek1@gmail.com,
       xiaoming.jiang@uni-ulm.de, jiang.xiaoming1984@gmail.com 

===================================================================*/

#ifndef My_paramStructure65trfz8btgbhunhunj9mn0jmi0mkopo
#define My_paramStructure65trfz8btgbhunhunj9mn0jmi0mkopo

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "cuda_assert.hpp"

#include <cufft.h>
#include <cublas_v2.h>
#define BUZZ_SIZE 1024


#ifdef __linux__
#include <stdbool.h>
#endif
extern int gpu_index;
extern bool atomsFromExternal;

struct cst_t {
	float m0; // = 9.109389e-31;  // electron rest mass (kg)
	float c;  // = 299792458.0f;   // speed of light (m/s)
	float e;  // = 1.602177e-19;   // elementary charge (C)
	float h;  // = 6.626075e-34;   // Planck's constant (Js)
	float pi; // = 3.1416 (dimensionless)
};

struct aberration_t {
	float C1_0;  // defocus (m)
	float C1_1;  // angle of defocus (rad)
	float A1_0;  // magnitude of astigmatism (m)
	float A1_1;  // angle of astigmatism (rad)
	float A2_0;  
	float A2_1;
	float B2_0;
	float B2_1;
	float C3_0;
	float C3_1;
	float A3_0;
	float A3_1;
	float S3_0;
	float S3_1;
	float A4_0;
	float A4_1;
	float B4_0;
	float B4_1;
	float D4_0;
	float D4_1;
	float C5_0;
	float C5_1;
	float A5_0;
	float A5_1;
	float R5_0;
	float R5_1;
	float S5_0;
	float S5_1;
};

struct EM_t {
	float E0;                 // acceleration voltage (V)
	float gamma;              // From relativity theory: 1 + e*E0/m0/c^2;
	float lambda;             // electron wavelength (m), see De Graef p. 92
	float sigma;              // interaction constant (1/(Vm))
	aberration_t aberration;  // Microscope aberrations
	float defocspread;        // Temporal incoherence  Defocus spread for the temporal incoherence (m)
	float illangle;           // illumination half angle characterizing the spatial incoherence (rad)
	float mtfa;               // parameters of the MTF, see my article in Microscopy and microanalysis
	float mtfb;
	float mtfc;
	float mtfd;
	float ObjAp;              // DIAMETER (!!! (I'm so sorry...)) of the objective aperture (rad)
};

struct IM_t {
	int mode;
	int m1;           // 1st dimension of the reconstructed potential m1 = n1 + 2*dn1  number of columns (width)
	int m2;           // 2nd dimension of the reconstructed potential m2 = n2 + 2*dn2  number of rows (height)
	int m3;           // 3rd dimension of the reconstructed potential, i.e. number of slices
	float d1;         // pixelsize in 1st dimension of measurements and reconstruction (m)
	float d2;         // pixelsize in 2nd dimension of measurements and reconstruction (m)
	float d3;         // pixelsize in 3rd dimension of measurements and reconstruction (m)
	int dn1;          // additional edge around reconstruction in 1st dimension
	int dn2;          // additional edge around reconstruction in 2nd dimension
	int n1;           // 1st dimension of the measurements
	int n2;           // 2nd dimension of the measurements
	int n3;           // 3rd dimension of the measurements, i.e. the number of different tilt angles
	int frPh;       // Number of frozen phonon iterations
	float pD;         // mean number of electrons per pixel
	float subSlTh;    // The images are calculated with (approx.!) this slice thickness [m]
	float* tiltspec;  // tilt angle of the specimen for every measurement (rad) (n3 by 2)
	float* tiltbeam;  // tilt angle of the beam for every measurement (rad) (n3 by 2)
	float* defoci;    // Defoci in meters (n3 by 1)
	float specimen_tilt_offset_x;
	float specimen_tilt_offset_y;
	float specimen_tilt_offset_z;
	bool doBeamTilt;
};

struct SAMPLE_t{
	char sample_name[BUZZ_SIZE];
	char material[BUZZ_SIZE];
	float imPot;       //Imaginary potential factor to approximate absorption: V <- V + iV * absorptive_potential_factor
	int nAt;           // number of atoms
};

struct USER_t{
	char user_name[BUZZ_SIZE];
	char institution[BUZZ_SIZE];
	char department[BUZZ_SIZE];
	char email[BUZZ_SIZE];
};
struct COMMENT_t{
	char comments[BUZZ_SIZE];
};


struct CU_t {
	int gS;  // gridsize
	int gS2D;  // gridsize
	int bS;  // blocksize
	cufftHandle cufftPlan;
	cublasHandle_t cublasHandle;
};


struct params_t {
	cst_t cst;  // Nature's constants
	EM_t EM;    // Electron microscope parameters
	IM_t IM;    // Imaging parameters
	CU_t CU;    // CUDA parameters
	USER_t USER;
	COMMENT_t COMMENT;
	SAMPLE_t  SAMPLE;
};

bool readConfig(const char* file, params_t * params, int** Z_d, float** xyzCoord_d, float** DWF_d, float** occ_d  );

void writeConfig (const char* file, params_t* params, int** Z_d, float** xyzCoord_d, float** DWF_d, float** occ_d);

void defaultParams( params_t* params, int n3 );

bool getParams( const char* filename, params_t** params, int** Z_d, float** xyzCoord_d, float** DWF_d, float** occ_d  );

void consitentParams( params_t* params );

void allocParams( params_t** params, int n );

void freeParams( params_t** params );

void setGridAndBlockSize( params_t* params );

void setCufftPlan( params_t* params );

void setCublasHandle( params_t* params );

void copyParams( params_t* dst, params_t* src );

void freeDeviceParams( params_t** params, int n3 );

void setDeviceParams( params_t** params_d, params_t* params );

void getDeviceParams( params_t** params, params_t** params_d, int n3 );

void transposeMeasurements( float* I, params_t* params );

void myCudaMallocSubStruct( params_t* params, int n3 );

void myCudaFreeSubStruct( params_t* params );

void myCudaMemcpySubStruct( params_t* dst, params_t* src, int n3, enum cudaMemcpyKind kind );

int numberOfAtoms( FILE* fr );

void resetLine( char* line );

void readCoordinates ( FILE* fr, int* Z_h, float* xyzCoord_h, float* DWF_h, float* occ_h );

void printLevelOptimization(params_t* params);

void readAtomsFromArray(params_t* params, int** Z_d, float** xyzCoord_d, float** DWF_d, float** occ_d , float * atomsArray, int numAtoms );

void exportFormedimage(params_t* params, float * dstImage, float * srcImage );

extern int printLevel;

#endif 
