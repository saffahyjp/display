copy /b header+%1 tmp.cpp
g++ tmp.cpp -E -o tmpa
uar <tmpa >tmpb
type tmpb
t2b tmpb %2
