./head-writer.out > ./tmp.cpp && \
cat output/src-cp.cpp >> ./tmp.cpp && \
./stkn-core.out < $1 >> ./tmp.cpp && \
g++ tmp.cpp -o $2
