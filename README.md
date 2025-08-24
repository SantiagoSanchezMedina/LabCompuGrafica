# 1) Crea carpeta y entra
mkdir compu-grafica-practica-01
cd compu-grafica-practica-01

# 2) Inicializa repo y fija la rama principal como 'main'
git init
git branch -M main

# 3) Crea README.md (abrelo en tu editor y pega el template)
#   (o usa:  notepad README.md  en Windows)
notepad README.md

# 4) Primer commit
git add README.md
git commit -m "Add carátula en README"

# 5) Conecta con GitHub (crea un repo vacío con el mismo nombre y copia su URL)
git remote add origin https://github.com/<tu_usuario>/compu-grafica-practica-01.git
git push -u origin main
