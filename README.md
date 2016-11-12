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
create a folder in the project folder `mkdir build` and switch to it `cd build`. run cmake `cmake ..` to check dependencies and create the Makefile. build the project with `make`.

#### how to use
TeleTest reads from stdin. therefore you can run it simply by `./teletest`, type in the assertions and execute with EOF (`Ctrl-D` on Linux and maxOS, `Ctrl-Z` on Windows). to run TeleTest in large scale you may want to read the assertions from a simple text file. in order to do that redirect stdin `./teletest < list_of_assertions.txt`

#### assertion syntax
teletest uses a simple syntax for testing using the keywords *RUN*, *WITH*, *EXPECT*. for example an assertion for a function `float add(float a, float b) {return a+b;}` could look like `RUN add WITH 3.5 -1.4 EXPECT 2.1`
`
