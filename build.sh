CLIENT_OUTPUT="build/client"
SERVER_OUTPUT="build/server"

function build {
    echo "Building $1";
    clang src/$1.cpp -o $2;
    echo "$1 build succeeded. Output to $2\n\n";
}

build "client" $CLIENT_OUTPUT
build "server" $SERVER_OUTPUT
