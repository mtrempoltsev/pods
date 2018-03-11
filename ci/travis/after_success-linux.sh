if [ "$PODS_COVERAGE" = "YES" ]; then
    lcov --directory . --capture --output-file coverage.info
    lcov --remove coverage.info '/usr/*' '*/externals/*' '*/examples/*' '*/tests/*' '*/benchmarks/*' --output-file coverage.info
    lcov --list coverage.info
    coveralls-lcov --repo-token=${COVERALLS_REPO_TOKEN} coverage.info
fi
