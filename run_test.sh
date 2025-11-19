#!/bin/bash

echo "🧪 TESTING MINISHELL..."
echo ""

test_num=0
passed=0

test_cmd() {
    test_num=$((test_num + 1))
    echo "Test $test_num: $1"
    eval "$2"
    if [ $? -eq 0 ]; then
        passed=$((passed + 1))
        echo "✅ PASS"
    else
        echo "❌ FAIL"
    fi
    echo ""
}

# Esegui i test
test_cmd "Echo base" 'printf "echo test\nexit\n" | ./minishell | grep -q "test"'
test_cmd "Variabile" 'printf "export X=1\necho \$X\nexit\n" | ./minishell | grep -q "1"'
test_cmd "Pipe" 'printf "echo abc | cat\nexit\n" | ./minishell | grep -q "abc"'
test_cmd "Redirect" 'printf "echo x > /tmp/t.txt\ncat /tmp/t.txt\nrm /tmp/t.txt\nexit\n" | ./minishell | grep -q "x"'
test_cmd "AND operator" 'printf "true && echo ok\nexit\n" | ./minishell | grep -q "ok"'
test_cmd "OR operator" 'printf "false || echo ok\nexit\n" | ./minishell | grep -q "ok"'
test_cmd "Exit status" 'printf "true\necho \$?\nexit\n" | ./minishell | grep -q "0"'
test_cmd "Quote rimosse" 'printf "echo \"test\"\nexit\n" | ./minishell | grep -q "^test$"'

echo "=========================================="
echo "RISULTATI: $passed/$test_num test passati"
echo "=========================================="