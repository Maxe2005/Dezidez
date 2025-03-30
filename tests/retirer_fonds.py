from PIL import Image

# Charger l'image
image_path = 'Ressources/Fleche_retour.jpg'
image = Image.open(image_path)

# Convertir l'image en mode RGBA
image = image.convert("RGBA")

# Obtenir les données de l'image
datas = image.getdata()

new_data = []
# Remplacer le blanc (ou toute autre couleur de fond) par une transparence
for item in datas:
    # Change all white (also shades of whites)
    # to transparent
    if item[:3] == (255, 255, 255):  # Assuming white background
        new_data.append((255, 255, 255, 0))  # Set the alpha to 0
    else:
        new_data.append(item)

# Mettre à jour les données de l'image
image.putdata(new_data)

# Sauvegarder l'image avec un fond transparent
transparent_image_path = 'fleche_demi_tour_transparent.png'
image.save(transparent_image_path, format='PNG')
