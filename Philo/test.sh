#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Test counter
TEST_NUMBER=1

# Configuration arrays for test parameters
PHILOSOPHERS=(1 2 3 4 5 10 20 50 100 150 200)
TIME_TO_DIE=(60 100 200 300 400 500 600 800 1000 1500 2000)
TIME_TO_EAT=(60 100 150 200 250 300)
TIME_TO_SLEEP=(60 100 150 200 250 300)
MEALS=(1 2 3 4 5 10 15 20)

# Function to print test header
print_test_header() {
    printf "${BLUE}Test #${TEST_NUMBER}: $1${NC}\n"
    ((TEST_NUMBER++))
}

# Function to get timestamp in milliseconds
get_timestamp() {
    date +%s%3N
}

# Function to check death timing precision
check_death_timing() {
    local output=$1
    local time_to_die=$2
    
    start_time=$(echo "$output" | head -n 1 | cut -d' ' -f1)
    death_time=$(echo "$output" | grep "died" | cut -d' ' -f1)
    
    if [ -n "$death_time" ] && [ -n "$start_time" ]; then
        actual_time=$((death_time - start_time))
        expected_time=$time_to_die
        difference=$((actual_time - expected_time))
        
        if [ $difference -gt 10 ]; then
            return 1
        fi
    fi
    return 0
}

# Function to kill process after timeout
kill_after() {
    local pid=$1
    local timeout=$2
    (
        sleep $timeout
        kill -9 $pid 2>/dev/null
    ) &
    return $!
}

# Function to determine expected result
determine_expected_result() {
    local philos=$1
    local die=$2
    local eat=$3
    local sleep=$4
    
    # If time_to_die is less than time_to_eat + time_to_sleep
    if [ $die -le $((eat + sleep)) ]; then
        echo "death"
    # If single philosopher
    elif [ $philos -eq 1 ]; then
        echo "death"
    else
        echo "alive"
    fi
}

# Function to run test with timeout
run_test() {
    local params=$1
    local timeout=$2
    local expected_result=$3
    local description=$4
    
    print_test_header "$description"
    printf "Parameters: $params\n"
    printf "Timeout: ${timeout}s\n"
    
    # Start the program
    ./philo $params > output.tmp 2>&1 &
    philo_pid=$!
    
    # Set up timeout
    kill_after $philo_pid $timeout
    killer_pid=$!
    
    # Wait for philo to finish or be killed
    wait $philo_pid 2>/dev/null
    exit_code=$?
    
    # Kill the timeout killer
    kill -9 $killer_pid 2>/dev/null
    
    # Read output
    output=$(<output.tmp)
    
    # Check timing precision if death occurred
    if echo "$output" | grep -q "died"; then
        time_to_die=$(echo $params | cut -d' ' -f2)
        check_death_timing "$output" "$time_to_die"
        timing_result=$?
        if [ $timing_result -ne 0 ]; then
            printf "${RED}Failed timing check${NC}\n"
        fi
    fi
    
    # Check message format
    if ! echo "$output" | grep -qE "^[0-9]+ [0-9]+ (has taken a fork|is eating|is sleeping|is thinking|died)$"; then
        printf "${RED}Invalid message format detected${NC}\n"
    fi
    
    # Check for specific conditions
    if echo "$output" | grep -q "died"; then
        if [ "$expected_result" = "death" ]; then
            printf "${GREEN}✓ Expected death occurred${NC}\n"
        else
            printf "${RED}✗ Unexpected death${NC}\n"
        fi
    elif [ "$expected_result" = "death" ]; then
        printf "${RED}✗ Expected death but all survived${NC}\n"
    else
        printf "${GREEN}✓ Expected survival confirmed${NC}\n"
    fi
    
    rm -f output.tmp
    printf "\n"
}

# Function to check build
check_build() {
    printf "${BLUE}Checking build...${NC}\n"
    make re > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        printf "${RED}Build failed!${NC}\n"
        exit 1
    fi
    printf "${GREEN}Build successful${NC}\n"
}

# Function to run predefined test cases
run_predefined_tests() {
    # Required test cases from subject
    run_test "1 800 200 200" 3 "death" "Single philosopher (should die)"
    run_test "5 800 200 200" 5 "alive" "Five philosophers (should survive)"
    run_test "5 800 200 200 7" 7 "alive" "Five philosophers with 7 meals"
    run_test "4 410 200 200" 5 "alive" "Four philosophers (should survive)"
    run_test "4 310 200 100" 5 "death" "Four philosophers (should die)"
    
    # Error cases
    run_test "0 800 200 200" 1 "error" "Zero philosophers"
    run_test "201 800 200 200" 1 "error" "Too many philosophers"
    run_test "4 59 200 200" 1 "error" "Time too low"
}

# Function to generate random test cases
generate_random_tests() {
    local num_tests=$1
    local count=0
    
    while [ $count -lt $num_tests ]; do
        # Randomly select parameters from arrays
        philos=${PHILOSOPHERS[$RANDOM % ${#PHILOSOPHERS[@]}]}
        die=${TIME_TO_DIE[$RANDOM % ${#TIME_TO_DIE[@]}]}
        eat=${TIME_TO_EAT[$RANDOM % ${#TIME_TO_EAT[@]}]}
        sleep=${TIME_TO_SLEEP[$RANDOM % ${#TIME_TO_SLEEP[@]}]}
        
        # 30% chance to add number of meals
        if [ $((RANDOM % 100)) -lt 30 ]; then
            meals=${MEALS[$RANDOM % ${#MEALS[@]}]}
            params="$philos $die $eat $sleep $meals"
            description="Random test with meals: $philos philos, ${die}ms die, ${eat}ms eat, ${sleep}ms sleep, $meals meals"
        else
            params="$philos $die $eat $sleep"
            description="Random test: $philos philos, ${die}ms die, ${eat}ms eat, ${sleep}ms sleep"
        fi
        
        expected_result=$(determine_expected_result $philos $die $eat $sleep)
        timeout=$((die / 1000 + 2))
        
        run_test "$params" $timeout "$expected_result" "$description"
        ((count++))
    done
}

# Function to run systematic test cases
generate_systematic_tests() {
    # Test each philosopher count with various timing combinations
    for p in ${PHILOSOPHERS[@]}; do
        for d in 400 800; do
            for e in 200; do
                for s in 200; do
                    params="$p $d $e $s"
                    expected_result=$(determine_expected_result $p $d $e $s)
                    timeout=$((d / 1000 + 2))
                    description="Systematic test: $p philos, ${d}ms die, ${e}ms eat, ${s}ms sleep"
                    run_test "$params" $timeout "$expected_result" "$description"
                done
            done
        done
    done
}

# Main testing sequence
main() {
    # Check build first
    check_build
    
    printf "\n${YELLOW}Starting extensive testing suite...${NC}\n\n"
    
    # Run predefined test cases
    printf "${BLUE}Running predefined test cases...${NC}\n\n"
    run_predefined_tests
    
    # Run systematic test cases
    printf "${BLUE}Running systematic test cases...${NC}\n\n"
    generate_systematic_tests
    
    # Run random test cases (adjust number as needed)
    printf "${BLUE}Running random test cases...${NC}\n\n"
    generate_random_tests 450
    
    printf "\n${YELLOW}Testing complete! Total tests run: ${TEST_NUMBER}${NC}\n"
}

# Handle script arguments
case "$1" in
    "basic")
        check_build
        run_test "4 410 200 200" 5 "alive" "Basic test"
        ;;
    "random")
        check_build
        generate_random_tests 50
        ;;
    *)
        main
        ;;
esac

2 400 200 200
3 500 250 200
5 400 150 60
50 400 150 150
10 500 200 250
150 500 250 100
200 600 200 150
100 600 300 200
150 400 200 60 3
 200 500 300 60
 1 600 60 100 1
 2 300 200 60 2
 10 500 200 150
 3 800 250 200
 1 60 150 300 1
 100 400 200 100 20
 50 500 300 100
 10 500 250 100
 10 600 300 300
 200 500 300 60 5
 10 400 100 300 1
  3 300 200 60
  3 600 250 150
  200 800 150 15
   1 800 250 300 1