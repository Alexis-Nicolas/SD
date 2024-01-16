package Jeu;

public class Grille {
    private int taille;
    private int tailleTraitement;
    
	private int[][][] tab;

	private int[][][] tabTraitement;
    private int nbPassage;
    public int getTailleTraitement() {
		return tailleTraitement;
	}

	public void setTailleTraitement(int tailleTraitement) {
		this.tailleTraitement = tailleTraitement;
	}

	public int getTaille() {
		return taille;
	}

	public void setTaille(int taille) {
		this.taille = taille;
	}


    public int[][][] getTabTraitement() {
		return tabTraitement;
	}

	public void setTabTraitement(int[][][] tabTraitement) {
		this.tabTraitement = tabTraitement;
	}
	
	 public int[][][] getTab() {
			return tab;
		}

		public void setTab(int[][][] tab) {
			this.tab = tab;
		}


	public Grille(int taille) {
        this.taille = taille;
        this.tailleTraitement = taille + 2;
    }

	//initialise le tableau de traitement
    public int[][][] initTabTraitement() {
        this.tabTraitement = new int[tailleTraitement][tailleTraitement][tailleTraitement];
        for (int i = 0; i < tailleTraitement; i++) {
            for (int j = 0; j < tailleTraitement; j++) {
                for (int k = 0; k < tailleTraitement; k++) {
                    this.tabTraitement[i][j][k] = 0;
                }
            }
        }
        return this.tabTraitement;
    }

    
    //initialise le tableau de données
    public int[][][] initTab(int valeurRandom) {
        this.tab = new int[taille][taille][taille];
        int valeurCase = 0;
        for (int i = 1; i < taille + 1; i++) {
            for (int j = 1; j < taille + 1; j++) {
                for (int k = 1; k < taille + 1; k++) {
                    valeurCase = (int) (Math.random() * valeurRandom);
                    if (valeurCase == 1) {
                        this.tab[i - 1][j - 1][k - 1] = 1;
                        this.tabTraitement[i][j][k] = 1;
                    } else {
                        this.tab[i - 1][j - 1][k - 1] = 0;
                    }
                }
            }
        }
        return this.tab;
    }
	/**
	 * retourne la prochaine valeur d'une case à partir de ses voisins 
	 * @param tab tableau de traitement
	 * @param x adresse en x de la case à traiter 
	 * @param y adresse en y de la case à traiter 
	 * @param z adresse en z de la case à traiter 
	 * @return
	 */
    public int getProchaineValeur(int[][][] tab, int x, int y, int z) {
        int cpt = 0;
        boolean vivant = false;
        int retour = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (i == 1 && j == 1 && k == 1) {
                        if (tab[x][y][z] == 1) {
                            vivant = true;
                        }
                    }
                    if (tab[i][j][k] == 1) {
                        cpt++;
                    }
                }
            }
        }
        if ((vivant && (cpt == 2*3 || cpt == 3*3)) || (!vivant && cpt == 3*3)) {
            retour = 1;
        }
        return retour;
    }

    //affiche face par face le tableau 
    public String toString(int[][][] tab, int typeAff) {
        String retour = "";
        int tailleAff;
        if (typeAff == 1) {
            tailleAff = this.tailleTraitement;
        } else {
            tailleAff = this.taille;
        }
        for (int i = 0; i < tailleAff; i++) {
            retour += "\n*************************";
            for (int j = 0; j < tailleAff; j++) {
                retour += "\n|";
                for (int k = 0; k < tailleAff; k++) {
                    retour += "-" + tab[i][j][k];
                }
            }
        }
        return retour;
    }

    //realise l'appel du traitement pour chaque case du tableau
    public int[][][] getTabTraite(int[][][] tab) {
        for (int i = 1; i < this.getTailleTraitement() - 2; i++) {
            for (int j = 1; j < this.getTailleTraitement() - 2; j++) {
                for (int k = 1; k < this.getTailleTraitement() - 2; k++) {
                    tab[i][j][k] = this.getProchaineValeur(this.getTabTraitement(), i, j, k);
                }
            }
        }
        return tab;
    }
    
  //convertie un tableau de traitement en tableau de donnee
    public int[][][] convertTabToTabTraitement(int[][][] tab) {
        for (int i = 1; i < this.getTailleTraitement() - 2; i++) {
            for (int j = 1; j < this.getTailleTraitement() - 2; j++) {
                for (int k = 1; k < this.getTailleTraitement() - 2; k++) {
                    this.tabTraitement[i][j][k] = tab[i-1][j-1][k-1];
                }
            }
        }
        return tab;
    }

    

    public static void main(String[] args) {
    	System.out.println("bonojour");
        Grille g = new Grille(3);
        g.setTabTraitement(g.initTabTraitement());
        g.setTab(g.initTab(g.getTaille()));
        
        g.setTab(g.getTabTraite(g.getTabTraitement()));
        g.setTabTraitement(g.convertTabToTabTraitement(g.getTab()));
        
        g.toString();
        
        

    }
}
