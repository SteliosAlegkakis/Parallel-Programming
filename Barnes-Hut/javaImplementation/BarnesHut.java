package javaImplementation;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class BarnesHut {
    private static double dimension;
    private static ArrayList<Body> bodies;
    private static ArrayList<Square> BHtree;

    public static void main(String[] args) {
        bodies = new ArrayList<Body>();
        BHtree = new ArrayList<Square>();
        readFile(args[0]);

        Square root = new Square(0, -1, 0, 0.0, 0.0, 2*dimension, "root");
        root.bodies = bodies;
        BHtree.add(root);
        buildTree(root.bodies, root);
        printTree();
    }



    public static boolean isBodyInSquare(Body b, Square s){
        return b.x <= s.x_mid + s.length/2.0 && b.x >= s.x_mid - s.length/2.0 && b.y <= s.y_mid + s.length/2.0 && b.y >= s.y_mid - s.length/2.0;
    }

    public static void buildTree(ArrayList<Body> bodies, Square parent){

        Square top_left = new Square(parent.id + 1, parent.id, parent.level + 1, parent.x_mid - parent.length/4.0, parent.y_mid + parent.length/4.0, parent.length/2.0, "top_left");
        BHtree.add(top_left);
        for(Body b : bodies) {
            if(isBodyInSquare(b, top_left)) {
                top_left.bodies.add(b);
            }
        }
        if(top_left.bodies.size() > 1){
            buildTree(top_left.bodies, top_left);
        }

        Square top_right = new Square(parent.id + 2, parent.id, parent.level + 1, parent.x_mid + parent.length/4.0, parent.y_mid + parent.length/4.0, parent.length/2.0, "top_right");
        BHtree.add(top_right);
        for(Body b : bodies) {
            if(isBodyInSquare(b, top_right)){
                top_right.bodies.add(b);
            }
        }
        if(top_right.bodies.size() > 1){
            buildTree(top_right.bodies, top_right);
        }

        Square bottom_left = new Square(parent.id + 3, parent.id, parent.level + 1, parent.x_mid - parent.length/4.0, parent.y_mid - parent.length/4.0, parent.length/2.0, "bottom_left");
        BHtree.add(bottom_left);
        for(Body b : bodies) {
            if(isBodyInSquare(b, bottom_left)) {
                bottom_left.bodies.add(b);
            }
        }
        if(bottom_left.bodies.size() > 1){
            buildTree(bottom_left.bodies, bottom_left);
        }

        Square bottom_right = new Square(parent.id + 4, parent.id, parent.level + 1, parent.x_mid + parent.length/4.0, parent.y_mid - parent.length/4.0, parent.length/2.0, "bottom_right");
        BHtree.add(bottom_right);
        for(Body b : bodies) {
            if(isBodyInSquare(b, bottom_right)) {
                bottom_right.bodies.add(b);
            }
        }
        if(bottom_right.bodies.size() > 1){
            buildTree(bottom_right.bodies, bottom_right);
        }
    }

    public static void readFile(String filename){
        try {
            File file = new File(filename);
            Scanner scanner = new Scanner(file);
            int number_of_bodies = Integer.parseInt(scanner.nextLine());
            dimension = Double.parseDouble(scanner.nextLine());
            int n = 0;
            while (scanner.hasNextLine() && n < number_of_bodies) {
                String line = scanner.nextLine();
                String[] values = line.split(" ");
                double x = Double.parseDouble(values[0]);
                double y = Double.parseDouble(values[1]);
                double vx = Double.parseDouble(values[2]);
                double vy = Double.parseDouble(values[3]);
                double mass = Double.parseDouble(values[4]);
                String s = values[5];
                Body body = new Body(n, x, y, vx, vy, mass, s);
                bodies.add(body);
                n++;
            }
            
            scanner.close();
        } catch (FileNotFoundException e) {
            System.out.println("File not found: " + filename);
        }
        return ;
    }

    public static void printTree() {
        System.out.println("dimension: " + dimension);
        for(Square s : BHtree){
            System.out.println("id: " + s.id + " parent_id: " + s.parent_id + " level: " + s.level + " x_mid: " + s.x_mid + " y_mid: " + s.y_mid + " length: " + s.length + " location: " + s.location + " bodies: " + s.bodies.size());
            for(Body b : s.bodies){
                System.out.println(b.s);
            }
        }
    
    }
}

class Body {
    int id;
    double x;
    double y;
    double vx;
    double vy;
    double mass;
    String s;
    double fx;
    double fy;

    Body(int id, double x, double y, double vx, double vy, double mass, String s){
        this.id = id;
        this.x = x;
        this.y = y;
        this.mass = mass;
        this.vx = vx;
        this.vy = vy;
        this.s = s;
    }

    public String toString(){
        return "x: " + x + " y: " + y + " vx: " + vx + " vy: " + vy + " mass: " + mass + " s: " + s + " fx: " + fx + " fy: " + fy;
    }
}

class Square {
    int id;
    int parent_id;
    int level;
    double x_mid;
    double y_mid;
    double length;
    String location;
    ArrayList<Body> bodies;
    Square top_left;
    Square top_right;
    Square bottom_left;
    Square bottom_right;

    Square(int id, int parent_id, int level, double x_mid, double y_mid, double length, String location){
        this.id = id;
        this.parent_id = parent_id;
        this.level = level;
        this.x_mid = x_mid;
        this.y_mid = y_mid;
        this.length = length;
        this.location = location;
        this.bodies = new ArrayList<Body>();
    }
}