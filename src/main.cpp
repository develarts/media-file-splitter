#include <stdio.h>
#include <popt.h>
#include <string.h>

#include "app.h"

using namespace MediaFileSplitter;

void usage(poptContext optCon, int exitcode, char *error)
{
    poptPrintUsage(optCon, stderr, 0);
    if (error) {
        fprintf(stderr, "%s\n", error);
    }
}

bool parseOptions(poptContext optCon, appOptions* options)
{
    char c;

    while ((c = poptGetNextOpt(optCon)) >= 0) {
        switch (c)
        {
            case 'b':
                options->backup = poptGetOptArg(optCon);
                options->clean = true;
                break;
            case 'c':
                options->clean = true;
                break;
            case 's':
                options->silent = true;
                break;
        }
    }

    options->path = poptGetArg(optCon);

    if (options->path == NULL) {
        usage(optCon, 1, (char*)"Specify index file or directory to scan");
        return false;
    }

    return true;
}

int main(int argc, const char *argv[])
{
    poptContext optCon;
    appOptions options;

    options.clean = false;
    options.silent = false;

    struct poptOption optionsTable[] = {
        { "backup", 'b', POPT_ARG_STRING, 0, 'b',
          "path to directory where save backup files" },
        { "clean", 'c', 0, 0, 'c',
          "do you want to delete exploded files" },
        { "silent", 's', 0, 0, 's',
          "no verbose output" },
        POPT_AUTOHELP { NULL, 0, 0, NULL, 0 }
    };

    optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
    poptSetOtherOptionHelp(optCon, "<file or directory>");

    if (parseOptions(optCon, &options)) {

        try {

        	App* app = new App(options);
        	app->start();

        } catch (const char* e) {

            printf("error: %s\n", e);
        }
    }

    poptFreeContext(optCon);
    return 0;
}
