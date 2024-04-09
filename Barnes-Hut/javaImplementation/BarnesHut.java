package javaImplementation;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

class BarnesHut{

    static ArrayList<Particle> particles = new ArrayList<Particle>();
    static int numberOfParticles = 0;
    static double dimension = 0.0;
    
    public static void main(String[] args){
        System.out.println("Barnes-Hut Algorithm");
        if (args.length == 0) {
            System.out.println("No arguments provided. Please provide a filename.");
            return;
        }
        readFile(args[0]);
        print();
        barnesHut();
    }

    public static void barnesHut(){
        System.out.println("Barnes-Hut Algorithm");
    }

    public static int getLength(){
        return 0;
    }

    public static void insert(){
        return ;
    }

    public static double netForce(){
        return 0.0;
    }

    public static void newPosition(){
        return ;
    }

    public static void print() {
        System.out.println("Number of Particles: " + numberOfParticles);
        for (Particle particle : particles) {
            System.out.println(particle.toString());
        }
    }

    public static void readFile(String filename){
        try {
            File file = new File(filename);
            Scanner scanner = new Scanner(file);
            numberOfParticles = Integer.parseInt(scanner.nextLine());
            dimension = Double.parseDouble(scanner.nextLine());
            int n = 0;
            while (scanner.hasNextLine() && n < numberOfParticles) {
                String line = scanner.nextLine();
                String[] values = line.split(" ");
                double x = Double.parseDouble(values[0]);
                double y = Double.parseDouble(values[1]);
                double vx = Double.parseDouble(values[2]);
                double vy = Double.parseDouble(values[3]);
                double mass = Double.parseDouble(values[4]);
                String s = values[5];
                Particle particle = new Particle(x, y, vx, vy, mass, s);
                particles.add(particle);
                n++;
            }
            
            scanner.close();
        } catch (FileNotFoundException e) {
            System.out.println("File not found: " + filename);
        }
        return ;
    }
}