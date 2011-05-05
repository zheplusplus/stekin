if [ "-cm" == $1 ];
then
    CHECK_MEMORY="valgrind --log-file=tmp.log.memcheck --leak-check=full"
    INPUT=$2
    OUTPUT=$3
else
    INPUT=$1
    OUTPUT=$2
fi
$CHECK_MEMORY ./head-writer.out > ./tmp.cpp && \
cat output/src-cp.cpp >> ./tmp.cpp && \
$CHECK_MEMORY ./stkn-core.out < $INPUT >> ./tmp.cpp && \
g++ tmp.cpp -o $OUTPUT
