mkdir build
cd build
cmake ..

make Tests 
Tests/Tests                        //run tests


make examples 
examples/examples                  //run examples


make CommandInterface
CommandInterface/CommandInterface //run command interface

make Graphics                     //compile Graphics library
make Geometry                     //compile Geometry library

make docs
cd ../Docs/html
open index.html                   //open documentation
