#!/bin/bash

# ============================================================================ #
#                    MINISHELL CORRECTION TEST SCRIPT                          #
# ============================================================================ #
# Questo script testa tutte le funzionalità richieste dalla scala di           #
# correzione di minishell. Esegui con: ./test_correction.sh                    #
# ============================================================================ #

# Colori per output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Contatori
PASSED=0
FAILED=0
TOTAL=0

# Path del minishell
MINISHELL="./minishell"

# Funzione per stampare header di sezione
print_header() {
    echo ""
    echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════${NC}"
    echo -e "${BOLD}${BLUE}  $1${NC}"
    echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════${NC}"
}

# Funzione per stampare sottosezione
print_subheader() {
    echo ""
    echo -e "${CYAN}────────────────────────────────────────────────────────────────${NC}"
    echo -e "${CYAN}  $1${NC}"
    echo -e "${CYAN}────────────────────────────────────────────────────────────────${NC}"
}

# Funzione per stampare test
print_test() {
    echo -e "${YELLOW}TEST:${NC} $1"
}

# Funzione per stampare istruzioni manuali
print_manual() {
    echo -e "${MAGENTA}[MANUALE]${NC} $1"
}

# Funzione per eseguire test automatico
run_test() {
    local description="$1"
    local command="$2"
    local expected="$3"
    
    TOTAL=$((TOTAL + 1))
    
    # Esegui in minishell
    result=$(echo "$command" | $MINISHELL 2>&1 | grep -v "^minishell\$" | grep -v "^exit$")
    
    # Esegui in bash per confronto
    bash_result=$(echo "$command" | bash 2>&1)
    
    echo -e "${YELLOW}TEST $TOTAL:${NC} $description"
    echo -e "  Comando: ${CYAN}$command${NC}"
    
    if [ -n "$expected" ]; then
        if echo "$result" | grep -q "$expected"; then
            echo -e "  ${GREEN}✓ PASS${NC}"
            PASSED=$((PASSED + 1))
        else
            echo -e "  ${RED}✗ FAIL${NC}"
            echo -e "  Atteso: $expected"
            echo -e "  Ottenuto: $result"
            FAILED=$((FAILED + 1))
        fi
    else
        echo -e "  Minishell: $result"
        echo -e "  Bash:      $bash_result"
        if [ "$result" = "$bash_result" ]; then
            echo -e "  ${GREEN}✓ PASS (identico a bash)${NC}"
            PASSED=$((PASSED + 1))
        else
            echo -e "  ${YELLOW}⚠ Verificare manualmente${NC}"
        fi
    fi
}

# ============================================================================ #
#                              INIZIO TEST                                     #
# ============================================================================ #

echo -e "${BOLD}${GREEN}"
echo "╔══════════════════════════════════════════════════════════════════╗"
echo "║           MINISHELL CORRECTION SHEET TEST SCRIPT                 ║"
echo "║                    Test Automatici e Manuali                     ║"
echo "╚══════════════════════════════════════════════════════════════════╝"
echo -e "${NC}"

# Verifica che minishell esista
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}ERRORE: $MINISHELL non trovato! Esegui 'make' prima.${NC}"
    exit 1
fi

# ============================================================================ #
print_header "COMPILAZIONE"
# ============================================================================ #

print_test "Verifica flag di compilazione (-Wall -Wextra -Werror)"
echo -e "${CYAN}Eseguo: make -n | head -20${NC}"
make -n 2>&1 | head -20

echo ""
print_test "Verifica che non faccia re-link"
echo -e "${CYAN}Eseguo: make && make${NC}"
make > /dev/null 2>&1
make_output=$(make 2>&1)
if echo "$make_output" | grep -q "Nothing to be done"; then
    echo -e "${GREEN}✓ PASS: Makefile non fa re-link${NC}"
else
    echo -e "${RED}✗ FAIL: Makefile fa re-link${NC}"
fi

# ============================================================================ #
print_header "SIMPLE COMMAND & GLOBAL VARIABLES"
# ============================================================================ #

print_subheader "Comandi con path assoluto"

print_manual "Testa: /bin/ls"
print_manual "Testa: /bin/pwd"
print_manual "Testa: /usr/bin/env"

print_subheader "Variabile globale"
print_manual "Verifica che ci sia UNA SOLA variabile globale"
echo -e "${CYAN}Cerca 'g_signal' nel codice - deve contenere SOLO il numero del segnale${NC}"
grep -r "g_signal" --include="*.c" --include="*.h" src/ includes/ 2>/dev/null | head -10

print_subheader "Comandi vuoti"
print_manual "Testa: (invio senza comando)"
print_manual "Testa: '     ' (solo spazi)"
print_manual "Testa: '		' (solo tab)"

# ============================================================================ #
print_header "ARGUMENTS"
# ============================================================================ #

run_test "ls con argomento -la" "ls -la" ""
run_test "echo con argomenti multipli" "echo hello world 42" "hello world 42"
run_test "cat con file" "echo test > /tmp/test_minishell.txt && cat /tmp/test_minishell.txt" "test"

# ============================================================================ #
print_header "ECHO"
# ============================================================================ #

run_test "echo senza argomenti" "echo" ""
run_test "echo con argomenti" "echo hello world" "hello world"
run_test "echo -n (no newline)" "echo -n hello" "hello"
run_test "echo -n -n -n (multipli -n)" "echo -n -n -n hello" "hello"
run_test "echo -nnn (flag combinato)" "echo -nnn hello" "hello"
run_test "echo -n seguito da testo" "echo -n hello; echo world" "helloworld"

# ============================================================================ #
print_header "EXIT"
# ============================================================================ #

print_manual "Testa i seguenti comandi (riavvia minishell dopo ogni exit):"
echo ""
echo "  exit                    -> dovrebbe uscire con l'ultimo exit code"
echo "  exit 0                  -> dovrebbe uscire con codice 0"
echo "  exit 42                 -> dovrebbe uscire con codice 42"
echo "  exit 255                -> dovrebbe uscire con codice 255"
echo "  exit 256                -> dovrebbe uscire con codice 0 (256 % 256)"
echo "  exit -1                 -> dovrebbe uscire con codice 255"
echo "  exit abc                -> errore 'numeric argument required', esce con 2"
echo "  exit 1 2                -> errore 'too many arguments', NON esce"

# ============================================================================ #
print_header "RETURN VALUE OF A PROCESS (\$?)"
# ============================================================================ #

print_manual "Testa manualmente in minishell:"
echo ""
echo "  /bin/ls"
echo "  echo \$?                 -> dovrebbe stampare 0"
echo ""
echo "  /bin/ls filethatdoesntexist"
echo "  echo \$?                 -> dovrebbe stampare un valore != 0"
echo ""
echo "  expr \$? + \$?"

# Test automatico per $?
# Nota: su macOS true e false sono in /usr/bin, non /bin
run_test "echo \$? dopo comando riuscito" "true; echo \$?" "0"
run_test "echo \$? dopo comando fallito" "false; echo \$?" "1"

# ============================================================================ #
print_header "SIGNALS"
# ============================================================================ #

print_manual "Test manuali dei segnali (MOLTO IMPORTANTI):"
echo ""
echo "  1. ctrl-C su prompt vuoto:"
echo "     -> Deve mostrare nuova linea con nuovo prompt"
echo ""
echo "  2. ctrl-\\ su prompt vuoto:"
echo "     -> Non deve fare nulla"
echo ""
echo "  3. ctrl-D su prompt vuoto:"
echo "     -> Deve uscire da minishell (stampa 'exit')"
echo "     -> RIAVVIA MINISHELL DOPO!"
echo ""
echo "  4. ctrl-C dopo aver scritto qualcosa:"
echo "     -> Nuova linea, nuovo prompt, buffer pulito"
echo "     -> Premi Enter per verificare che non esegua nulla"
echo ""
echo "  5. ctrl-D dopo aver scritto qualcosa:"
echo "     -> Non deve fare nulla"
echo ""
echo "  6. ctrl-\\ dopo aver scritto qualcosa:"
echo "     -> Non deve fare nulla"
echo ""
echo "  7. ctrl-C durante 'cat' senza argomenti:"
echo "     -> Deve terminare cat e tornare al prompt"
echo ""
echo "  8. ctrl-\\ durante 'cat' senza argomenti:"
echo "     -> Deve terminare cat con 'Quit: 3'"
echo ""
echo "  9. ctrl-D durante 'cat' senza argomenti:"
echo "     -> Deve terminare cat normalmente"

# ============================================================================ #
print_header "DOUBLE QUOTES"
# ============================================================================ #

run_test "echo con double quotes" 'echo "hello world"' "hello world"
run_test "echo con spazi in quotes" 'echo "hello    world"' "hello    world"
run_test "echo comando in quotes (letterale)" 'echo "cat lol.c | cat > lol.c"' "cat lol.c | cat > lol.c"
run_test "echo con variabile in quotes" 'export TEST=hello; echo "$TEST"' "hello"

# ============================================================================ #
print_header "SINGLE QUOTES"
# ============================================================================ #

run_test "echo con single quotes" "echo 'hello world'" "hello world"
run_test "echo \$USER in single quotes (letterale)" "echo '\$USER'" "\$USER"
run_test "echo con pipe in single quotes" "echo 'hello | world'" "hello | world"
run_test "echo con redirect in single quotes" "echo 'hello > world'" "hello > world"

# ============================================================================ #
print_header "ENV"
# ============================================================================ #

print_manual "Testa: env"
echo "Dovrebbe mostrare tutte le variabili d'ambiente correnti"

# ============================================================================ #
print_header "EXPORT"
# ============================================================================ #

run_test "export nuova variabile" "export TESTVAR=hello; echo \$TESTVAR" "hello"
run_test "export sovrascrittura" "export TESTVAR=first; export TESTVAR=second; echo \$TESTVAR" "second"

print_manual "Testa: export (senza argomenti)"
echo "Dovrebbe mostrare tutte le variabili nel formato: declare -x KEY=\"value\""

print_manual "Testa: export INVALID=VAR=NAME"
print_manual "Testa: export 123invalid"

# ============================================================================ #
print_header "UNSET"
# ============================================================================ #

run_test "unset variabile" "export TESTVAR=hello; unset TESTVAR; echo \$TESTVAR" ""

print_manual "Testa: unset PATH; ls (ls non dovrebbe funzionare)"

# ============================================================================ #
print_header "CD"
# ============================================================================ #

run_test "cd a /tmp e pwd" "cd /tmp; pwd" "/tmp"
run_test "cd a /tmp e /bin/ls" "cd /tmp; /bin/ls > /dev/null && echo OK" "OK"
run_test "cd con . (directory corrente)" "cd .; pwd" ""
run_test "cd con .." "cd /tmp; cd ..; pwd" "/"

print_manual "Testa: cd (senza argomenti) -> dovrebbe andare a HOME"
print_manual "Testa: cd - -> dovrebbe andare a OLDPWD e stampare il path"
print_manual "Testa: cd /path/che/non/esiste -> dovrebbe stampare errore"

# ============================================================================ #
print_header "PWD"
# ============================================================================ #

run_test "pwd" "pwd" ""
print_manual "Verifica che pwd stampi la directory corrente"

# ============================================================================ #
print_header "RELATIVE PATH"
# ============================================================================ #

print_manual "Testa comandi con path relativi:"
echo "  ./minishell"
echo "  ../../../bin/ls"

# ============================================================================ #
print_header "ENVIRONMENT PATH"
# ============================================================================ #

run_test "ls senza path (usa PATH)" "ls > /dev/null && echo OK" "OK"
run_test "wc senza path" "echo test | wc -c" ""

print_manual "Testa:"
echo "  unset PATH"
echo "  ls              -> dovrebbe fallire (command not found)"
echo "  export PATH=/bin:/usr/bin"
echo "  ls              -> dovrebbe funzionare"

# ============================================================================ #
print_header "REDIRECTION"
# ============================================================================ #

# Pulisci file di test
rm -f /tmp/minishell_test_* 2>/dev/null

run_test "redirect output >" "echo hello > /tmp/minishell_test_out.txt; cat /tmp/minishell_test_out.txt" "hello"
run_test "redirect output >> (append)" "echo line1 > /tmp/minishell_test_append.txt; echo line2 >> /tmp/minishell_test_append.txt; cat /tmp/minishell_test_append.txt" ""
run_test "redirect input <" "echo hello > /tmp/minishell_test_in.txt; cat < /tmp/minishell_test_in.txt" "hello"

print_manual "Testa heredoc (<<):"
echo "  cat << EOF"
echo "  hello"
echo "  world"
echo "  EOF"
echo "  -> Dovrebbe stampare 'hello' e 'world'"

print_manual "Testa redirect multipli:"
echo "  echo test > /tmp/file1.txt > /tmp/file2.txt"
echo "  -> Solo file2.txt dovrebbe contenere 'test'"

# Pulisci
rm -f /tmp/minishell_test_* 2>/dev/null

# ============================================================================ #
print_header "PIPES"
# ============================================================================ #

run_test "pipe semplice" "echo hello | cat" "hello"
run_test "pipe multipla" "echo hello | cat | cat" "hello"
run_test "ls | grep" "ls | grep minishell" ""
run_test "cat | grep | wc" "echo -e 'hello\nworld\nhello' | grep hello | wc -l" ""

print_manual "Testa:"
echo "  ls filethatdoesntexist | grep bla | more"
echo "  -> Dovrebbe mostrare errore di ls ma non crashare"
echo ""
echo "  cat | cat | ls"
echo "  -> Comportamento 'normale' (premi ctrl-C o ctrl-D per uscire)"

# ============================================================================ #
print_header "GO CRAZY AND HISTORY"
# ============================================================================ #

print_manual "Test manuali:"
echo ""
echo "  1. Scrivi un comando, premi ctrl-C, poi Enter"
echo "     -> Non deve eseguire nulla (buffer pulito)"
echo ""
echo "  2. Usa freccia SU e GIU' per navigare la history"
echo ""
echo "  3. Testa comando inesistente:"
echo "     dsbksdgbksdghsd"
echo "     -> Deve stampare errore, non crashare"
echo ""
echo "  4. Testa 'cat | cat | ls'"
echo ""
echo "  5. Testa comando lungo con molti argomenti"

# ============================================================================ #
print_header "ENVIRONMENT VARIABLES"
# ============================================================================ #

run_test "\$USER espansione" "echo \$USER" "$USER"
run_test "\$HOME espansione" "echo \$HOME" "$HOME"
run_test "Variabile in double quotes" 'echo "$USER"' "$USER"
run_test "Variabile non esistente" "echo \$VARIABILE_CHE_NON_ESISTE" ""

# ============================================================================ #
print_header "BONUS - AND, OR, PARENTHESES"
# ============================================================================ #

run_test "&& con successo" "true && echo success" "success"
run_test "&& con fallimento" "false && echo success" ""
run_test "|| con successo" "true || echo fallback" ""
run_test "|| con fallimento" "false || echo fallback" "fallback"
run_test "parentesi semplici" "(echo hello)" "hello"
run_test "parentesi con &&" "(echo first && echo second)" ""
run_test "combinazione &&, ||" "false && echo no || echo yes" "yes"

# ============================================================================ #
print_header "BONUS - WILDCARD (*)"
# ============================================================================ #

print_manual "Testa wildcard (se implementato):"
echo "  ls *.c"
echo "  echo *"
echo "  ls src/*.c"

# ============================================================================ #
print_header "BONUS - SURPRISE (Quote nidificate)"
# ============================================================================ #

echo ""
echo -e "${MAGENTA}Test quote nidificate (BONUS):${NC}"
echo ""

# Test in bash per confronto
echo "In BASH:"
echo "  echo \"'\$USER'\" stampa: '$(echo "'$USER'")'"
echo "  echo '\"\$USER\"' stampa: $(echo '"$USER"')"
echo ""
print_manual "Verifica che minishell faccia lo stesso:"
echo "  echo \"'\$USER'\"   -> dovrebbe stampare: '$USER'"
echo "  echo '\"\$USER\"'   -> dovrebbe stampare: \"\$USER\""

# ============================================================================ #
print_header "MEMORY LEAKS"
# ============================================================================ #

print_manual "Verifica memory leaks con valgrind o leaks:"
echo ""
echo "  Su Linux:"
echo "    valgrind --leak-check=full ./minishell"
echo ""
echo "  Su macOS:"
echo "    leaks --atExit -- ./minishell"
echo ""
echo "  Esegui alcuni comandi, poi 'exit' e verifica i leak"

# ============================================================================ #
print_header "SYNTAX ERRORS"
# ============================================================================ #

print_manual "Verifica che questi comandi generino errori di sintassi:"
echo ""
echo "  |"
echo "  | ls"
echo "  ls |"
echo "  ls | | grep"
echo "  ls && &&"
echo "  ls || ||"
echo "  ()"
echo "  (|)"
echo "  'unclosed quote"
echo "  \"unclosed double quote"

# ============================================================================ #
#                           RIEPILOGO FINALE                                   #
# ============================================================================ #

echo ""
echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════${NC}"
echo -e "${BOLD}${BLUE}                         RIEPILOGO                              ${NC}"
echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════${NC}"
echo ""
echo -e "Test automatici passati: ${GREEN}$PASSED${NC}/$TOTAL"
echo -e "Test automatici falliti: ${RED}$FAILED${NC}/$TOTAL"
echo ""
echo -e "${YELLOW}NOTA: Molti test richiedono verifica manuale!${NC}"
echo -e "${YELLOW}Segui le istruzioni [MANUALE] sopra per completare la valutazione.${NC}"
echo ""

# Pulisci file temporanei
rm -f /tmp/minishell_test_* 2>/dev/null
rm -f /tmp/test_minishell.txt 2>/dev/null

echo -e "${GREEN}Script completato!${NC}"

