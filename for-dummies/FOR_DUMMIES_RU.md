# Туториал для неискушенных

Этот туториал предназначен в первую очередь для тех, кому лень разбираться с установкой всего, что нужно

## Шаги

### Зарегестрируйте свой аккаунт на GitHub

Ваша учетная запись потребуется для сборки всего, что вам будет нужно

### Создать приватный репозиторий

Создайте репозиторий, который потребуется для запуска GitHub Actions

![create-repo-1](images/create-repo-1.png)

Укажите имя репозитория и пометьте его как закрытый, если вы не хотите делать ваши .jar-файлы и .dll общедоступными

![create-repo-2](images/create-repo-2.png)

### Импортируйте код из этого репозитория

Нажмите `Import` для импорта

![import-1](images/import-1.png)

Укажите URL этого репозитория, т.е. `https://github.com/radioegor146/jar-to-dll`

![import-2](images/import-2.png)

Дождитесь завершения импорта

![import-3](images/import-3.png)

После завершения перейдите в свой репозиторий

![import-4](images/import-4.png)

### Разрешите GitHub Actions

Переместитесь на вкладку `Settings`

![allow-gh-actions-1](images/allow-gh-actions-1.png)

Откройте меню `Actions`

![allow-gh-actions-2](images/allow-gh-actions-2.png)

Нажмите `General`

![allow-gh-actions-3](images/allow-gh-actions-3.png)

Установите `Allow all actions to be run`

![allow-gh-actions-4](images/allow-gh-actions-4.png)

### Enable GitHub Actions

Переместитесь на вкладку `Actions`

![enable-gh-actions-1](images/enable-gh-actions-1.png)

Нажмите `Enable Actions on this repository` что бы включить Actions

![enable-gh-actions-2](images/enable-gh-actions-2.png)

### Загрузите свой .jar-файл и запустите сборку .dll

Переместитесь на вкладку `Code`

![upload-1](images/upload-1.png)

Откройте меню `Add file`

![upload-2](images/upload-2.png)

Нажмите `Upload files` для загрузки вашего .jar-файла

![upload-3](images/upload-3.png)

Переименуйте свой .jar-файл на вашем компьютере в `input.jar` (ОЧЕНЬ ВАЖНО!) и перетащите его сюда

![upload-4](images/upload-4.png)

Убедитесь, что он загружен, и нажмите `Commit changes`

![upload-5](images/upload-5.png)

Как будет загружен файл, перейдите во вкладку `Actions`

![upload-6](images/upload-6.png)

Здесь нажмите на запущенный Action

![upload-7](images/upload-7.png)

Дождитесь его выполнения

![upload-8](images/upload-8.png)

Обновите страницу и убедитесь, что предупреждений нет. Теперь вы можете скачать свой .dll-файл и использовать его по своему усмотрению!

![upload-9](images/upload-9.png)