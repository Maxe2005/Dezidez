from PIL import Image, ImageFilter

def remove_background(image_path, output_path, threshold=200):
    # Charger l'image
    image = Image.open(image_path).convert("RGBA")

    # Obtenir les données de l'image
    datas = image.getdata()

    new_data = []
    # Remplacer les couleurs proches du blanc par une transparence
    for item in datas:
        # Vérifier si la couleur est proche du blanc
        if all(i > threshold for i in item[:3]):
            new_data.append((255, 255, 255, 0))  # RGBA transparente
        else:
            new_data.append(item)

    # Mettre à jour les données de l'image
    image.putdata(new_data)

    # Appliquer un lissage pour adoucir les bords
    image = image.filter(ImageFilter.SMOOTH)

    # Sauvegarder l'image avec un fond transparent
    image.save(output_path, format='PNG')

# Utilisation de la fonction
remove_background('Ressources/Fleche_retour.jpg', 'fleche_demi_tour_transparent_clean.png')
