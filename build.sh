OUTPUT_DIR="./build/"
SOURCE_DIR="./src/"
CLIENT_NAME="client"
SERVER_NAME="server"

if [ ! -d "build" ]; then
    echo "No build directory found. Creating one.\n"
    mkdir build
fi

function build {
    echo "Building $1"
    clang $SOURCE_DIR$1.cpp -o $2
    echo "$1 build succeeded. Output to $2\n"
}

if [ $# -eq 0 ]; then
    build "client" $OUTPUT_DIR$CLIENT_NAME
    build "server" $OUTPUT_DIR$SERVER_NAME
fi

while [[ $# -gt 0 ]]; do
    arg="$1"

    case $arg in
        -h|--help)
            echo "Usage: ./build.sh [options]"
            echo "If no options are specified, both the client and server will be built.\n"
            echo "Options:"
            echo "  -h, --help     Show this help message and exit"
            echo "  -x, --clean    Clean the build directory"
            echo "  -c, --client   Build the client"
            echo "  -s, --server   Build the server"
            exit 1
            ;;
        -x|--clean)
            echo "Cleaning build directory..."
            rm -rf $OUTPUT_DIR
            echo "Build directory cleaned."
            exit 1
            ;;
        -c|--client)
            build "client" $OUTPUT_DIR$CLIENT_NAME
            shift
            ;;
        -s|--server)
            build "server" $OUTPUT_DIR$SERVER_NAME
            shift
            ;;
        *)
            echo "Unknown option: $arg"
            exit 1
            ;;
    esac
done

exit 0
