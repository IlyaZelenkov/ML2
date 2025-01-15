Welcome to the c++ implementation of my ML project. 

08/01/2025: 
DataFrame.cpp:  will be changed to header file. 
- Created a dataframe class that acts like a python pandas DataFrame.
- Dataframe stored as a HashMap data structure.
- Overloaded operators to perform vector arithmetic operations.
- will be changed to header file. 


09/01/2025:
DataFrame.cpp: 
- Added statistical functions.
- min, max, mean, stdev, variance.


10/01/2025:
LinearRegression.cpp: will be changed to header file. 
- Implemented basic fit, predict.


13/01/2025: 
intrinsic_dot.cpp: will be changed to header file. 
- Implemented a SIMD dot/sum operation optimization.
- As of the above date, the code requires AVX2 feature support by the CPU.
  - Development environment: Intel i9 14900k.
  - Does not work on my old macbook yet :(.   
- Checks to make sure users system is compatable with intel CPU features.
- Operates on either 8 32bit or 4 64bit values at a time.
- Navigates to correct function based on numerical data type.
    - int32(int), float32(float), float64(double).


