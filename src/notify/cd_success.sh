#!/bin/bash

TELEGRAM_BOT_TOKEN="5945240757:AAFY6xLAPB3Pghn8aYU3wb0op_Uw3WXn5hQ"
CI_PROJECT_NAME="DO6_CICD-0"
CI_PROJECT_URL="https://repos.21-school.ru/students/DO6_CICD.ID_356283/aegoncat_student.21_school.ru/DO6_CICD-0/-"
TELEGRAM_USER_ID="529629900"
TIME="10"
URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
TEXT="Deploy status: CD stage completed successfully!%0A%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch: develop"

curl -s --max-time $TIME -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null