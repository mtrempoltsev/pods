lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info 'tests/*' '/usr/*' '../externals/*' '*gtest.*' '*CMakeFiles.*"' --output-file coverage.info
lcov --list coverage.info
coveralls-lcov --repo-token ${COVERALLS_TOKEN} coverage.info
