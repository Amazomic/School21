# Check that a parameter was passed
if [ $# -ne 1 ]; then
  echo "Error: script requires 1 parameter (1, 2, or 3)"
  exit 1
fi

if [ ! -f ../02/log.txt ]; then
echo похоже файла лога нет, выполни скрипт сначала из второго задания
exit 1
fi