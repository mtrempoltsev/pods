lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/externals/*' --output-file coverage.info
lcov --list coverage.info
coveralls-lcov --repo-token ${COVERALLS_TOKEN} coverage.info
