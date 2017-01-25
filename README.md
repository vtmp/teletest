# TeleTest

## Introduction
this is a university project for running testcases on a microcontroller. the assertions are transmitted via uart and the microcontroller just evaluates the function and send back the return value. this setup saves valuable memory and allows easy testing without unnecessary flashing.

#### disclaimer
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## Setup

#### build executable
create a folder in the project folder `mkdir build` and switch to it `cd build`. run cmake `cmake ..` to check dependencies and create the Makefile. build the project with `make`. optionally you can compile the project with additional unit-tests (gtest)

#### dependencies
the project depends on several 3rd-party libraries:
1) *Inih* is a lightweight library to read configuration files.
2) *Libserialport* is a multi-platform library for serial communication.
3) *GTest* is a common library for unit-tests written in c++, developed and maintained by google.


#### how to use
TeleTest reads from stdin. therefore you can run it simply by `./teletest`, type in the assertions and execute with EOF (`Ctrl-D` on Linux and maxOS, `Ctrl-Z` on Windows). to run TeleTest in large scale you may want to read the assertions from a simple text file. in order to do that redirect stdin `./teletest < list_of_assertions.txt`

#### assertion syntax
TeleTest uses a simple syntax for testing using the keywords *RUN*, *WITH*, *EXPECT*. for example an assertion for a function `float add(float a, float b) {return a+b;}` could look like `RUN add WITH 3.5 -1.4 EXPECT 2.1`

#### configuration
TeleTest expects the configuration file `.TeleTest.conf` in your home directory. The project directory contains an example of the configuration file: `example.conf`.

## Mode of Operation
The PC (client) sends the assertions as strings via the serial port to the micro controller (server). The program of the microcontroller runs the function respectively and send back the return value. This testable functions calls itself reside hard-coded on the microcontroller. If and error occur (invalid assertion string, invalid function call or invalid arguments) the micro controller returns an error code. In case of an transmission error the PC will resend the assertion.

## Missing Code and Known Problems
The code works only for the unit-tests. the device-specific utility functions for sending and receiving characters on the microcontroller are missing. the code for the micro controller has plenty dependencies to the standard library. the code as it is will not fit on the LPC812 processor and therefore further optimization is necessary. 
