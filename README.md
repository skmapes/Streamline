
<div    align="center">
    <h1 align="center">
        Streamline<br>
        <a href="https://github.com/skmapes/Streamline/graphs/contributors" alt="Contributors">
            <img src="https://img.shields.io/github/contributors/skmapes/Streamline?style=flat"/>
        </a>
        <a href="https://github.com/skmapes/Streamline/network/members" alt="Forks">
            <img src="https://img.shields.io/github/forks/skmapes/Streamline?style=flat"/>
        </a>
        <a href="https://github.com/skmapes/Streamline/issues" alt="Issues">
            <img src="https://img.shields.io/github/issues/skmapes/Streamline.svg?style=flat"/>
        </a>
    </h1>
    <p  align="center">
        Take control and streamline your live stream experience with an all-in-one simple to use program.
    </p>
</div>

## About Streamline

Streamline is a server utility that works as a middleman between OBS and the end user, with the express goal to make setting up and running streams easier for tournament organizers and any other streamer.

Streamline uses [CrowCpp](https://crowcpp.org/master/) to start an HTTP server, which performs the following:
- Serve a web UI tool to enter stream information
- Write stream information to files on disk
- Interact with OBS via the [OBS Websocket](https://github.com/obsproject/obs-websocket) plugin

The included web UI is a simple form that is then sent to the server to handle.

## Roadmap

- [x] Prototype Server and Web UI
- [ ] Functional Demo with Sample Overlays
- [ ] OBS Websocket Integration (Server Side)
    - As of now, this is can be simply implemented via the web UI. For a cleaner solution, this should be done server side with a client websocket library.
- [ ] start.gg Integration
    - To support autofill in any regard, this is required. Similar to the OBS Websocket Integration, this can be achieved with a server side HTTP client library.
- [ ] Crow Migration (?)
    - Depending on the library of choice for the start.gg Integration, the server itself may be rewritten using the same library.

## Installation

For most non-developers, downloading Streamline from the [Releases](https://github.com/SKMapes/Streamline/releases) is recommended.

Alternatively, you can build the project from source with the instructions below.

## Building

### Prerequisites
- Git
- Visual Studio (2019 or higher, shouldn't matter much)
- [CrowCpp](https://crowcpp.org/master/)
    - vcpkg is recommended, other methods are untested but should work

### Instructions

1. Clone the repository: `git clone https://github.com/skmapes/Streamline.git`
2. Open `src/Streamline/Streamline.sln` and build the project with `Ctrl + B`.
3. Copy `Streamline.exe` from either `src/Streamline/Debug/` or `src/Streamline/Release/` into the repository root.

...and that's it! You can run the executable from there, and it should open your default browser with the status page.

## FAQ

### Why do I have to copy the executable from src/Streamline/Debug/ or src/Streamline/Release/?

The server relies on a few static files that aren't present outside of the root folder, so for the program to properly serve the tool's HTML files and write the JSON files in the proper locations.
