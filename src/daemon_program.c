#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <argp.h>
#include "become_daemon.h"

const char *argp_program_version     = "Developement v0.1";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

static char doc[] = "Daemon program for conneting to tuya cloud.";

/* A description of the arguments we accept. */
static char args_doc[] = "DaviceID Device_Secret ProductID";

/* Used by main to communicate with parse_opt. */
struct arguments {
	char *args[3]; /* DeviceID, DeviceSecret & ProductID */
};

static struct argp_option options[] = {
  { 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key) {
	case ARGP_KEY_ARG:
		if (state->arg_num >= 3)
			/* Too many arguments. */
			argp_usage(state);

		arguments->args[state->arg_num] = arg;

		break;

	case ARGP_KEY_END:
		if (state->arg_num < 3)
			/* Not enough arguments. */
			argp_usage(state);
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv)
{
	struct arguments arguments;
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	int ret;
	const char *LOGNAME = "DAEMON_EXAMPLE";

	// turn this process into a daemon
	ret = become_daemon(0);
	if (ret) {
		syslog(LOG_USER | LOG_ERR, "error starting");
		closelog();
		return EXIT_FAILURE;
	}

	// we are now a daemon!
	// printf now will go to /dev/null

	// open up the system log
	openlog(LOGNAME, LOG_PID, LOG_USER);
	syslog(LOG_USER | LOG_INFO, "starting");

	// run forever in the background
	while (1) {
		sleep(5);
		syslog(LOG_USER | LOG_INFO, "running | { DeviceID = %s | Device_Secret = %s | ProductID = %s }", arguments.args[0], arguments.args[1], arguments.args[2]);
	}

	return EXIT_SUCCESS;
}