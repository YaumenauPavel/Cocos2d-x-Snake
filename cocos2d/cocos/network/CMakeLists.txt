if (ANDROID)
    set(COCOS_NETWORK_HEADER
        network/CCDownloader-android.h
        )
    set(COCOS_NETWORK_SRC
        network/HttpClient-android.cpp
        network/SocketIO.cpp
        network/WebSocket.cpp
        network/CCDownloader.cpp
        network/CCDownloader-android.cpp
        network/Uri.cpp
        )
elseif (APPLE)
    set(COCOS_NETWORK_HEADER
        network/CCDownloader-apple.h
        network/HttpAsynConnection-apple.h
        )
    set(COCOS_NETWORK_SRC
        network/CCDownloader-apple.mm
        network/HttpClient.cpp
        network/SocketIO.cpp
        network/WebSocket.cpp
        network/CCDownloader.cpp
        network/CCDownloader-curl.cpp
        network/Uri.cpp
        )
else ()
    set(COCOS_NETWORK_SRC
        network/HttpClient.cpp
        network/SocketIO.cpp
        network/WebSocket.cpp
        network/CCDownloader.cpp
        network/CCDownloader-curl.cpp
        network/Uri.cpp
        )
endif ()

set(COCOS_NETWORK_HEADER
    ${COCOS_NETWORK_HEADER}
    network/CCDownloader-curl.h
    network/CCIDownloaderImpl.h
    network/CCDownloader.h
    network/WebSocket.h
    network/Uri.h
    network/SocketIO.h
    network/HttpClient.h
    network/HttpResponse.h
    network/HttpRequest.h
    network/HttpCookie.h
    )
