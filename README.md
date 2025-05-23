# Plan9port Configuration

This is my personal plan9port configuration. It works across all platforms supported by plan9port.

## Installation

	$ cd $HOME
	$ git clone https://github.com/xmonk/p9p.git .p9p

Then add the following to your `.bashrc`:

	if [ -d $HOME/.p9p ]; then
	    . $HOME/.p9p/rc
	fi
Apply the changes and optionally run the install script:


	$ . $HOME/.bashrc
	$ ./.p9p/install    # Optional

## macOS-specific Features

If you're using macOS, the install script will:

- Install Plumb.app, 9term.app, Acme.app and Sam.app in /Applications
- Download and install useful tools from GitHub
- Generate a LaunchAgent for acme-lsp (if installed)

## Using acme-lsp with the `alsp` Command

The `alsp` command provides a convenient interface for managing acme-lsp:


	$ alsp -h    # Show help and available options

Common operations:

	$ alsp -L    # Load the acme-lsp launch agent
	$ alsp -s    # Start acme-lsp (requires acme to be running)
	$ alsp -t    # Check if acme-lsp is running
	$ alsp -T    # View recent log entries
	$ alsp -o    # Show paths to log files


You can also use other options like `-S` to stop the service, `-r` to restart it, and `-c` to truncate log files.

The configuration ensures log directories exist when starting the service, making it easier to manage your acme-lsp environment.
