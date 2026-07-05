# Jules Architecture Prompt: Ngspice Repository Mirror

The official repository for Ngspice is natively hosted on **SourceForge** (`git://git.code.sf.net/p/ngspice/ngspice`), where all official commits, bug tracking, and developer mailing lists happen.

However, because some modern CI/CD tools, agents (like Jules), and developers prefer GitHub, there are several automated GitHub mirrors that perfectly sync with that SourceForge repository every single day.

If you specifically need a GitHub URL for Jules to easily fetch or for you to fork into your own GitHub account, the most reliable and constantly updated mirror used by the community is:
👉 **https://github.com/imr/ngspice**
*(This is a direct, automated mirror of the SourceForge repo)*

There is also another very well-maintained daily mirror specifically set up for modern build pipelines:
👉 **https://github.com/danchitnis/ngspice-sf-mirror**

### What You Should Do for Jules:
For your `JULES_ARCHITECTURE_PROMPT.md`, using the official SourceForge Git link you already found is perfectly fine because CMake's FetchContent command natively understands git:// URLs just as well as GitHub URLs.

But if Jules throws a timeout error trying to reach SourceForge (which sometimes happens with cloud-based AI agents), you can safely swap it out in the prompt to use one of the GitHub mirrors:

```cmake
# If SourceForge times out, tell Jules to use this in the CMakeLists.txt instead:
FetchContent_Declare(
    ngspice
    GIT_REPOSITORY https://github.com/imr/ngspice.git
    GIT_TAG master
)
```

Both roads lead to the exact same raw C code for your SPICE engine!