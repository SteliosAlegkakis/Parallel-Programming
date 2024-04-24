package javaImplementation;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.FileWriter;
import java.io.IOException;

public class BarnesHut {
    private static double dimension;
    private static ArrayList<Body> bodies;
    private static ArrayList<Square> BHtree;
    private static double G = 6.674 * Math.pow(10, -11);

    public static void main(String[] args) {
        bodies = new ArrayList<Body>();
        BHtree = new ArrayList<Square>();
        readFile(args[0]);
        int itterations = Integer.parseInt(args[2]);

        for(int i = 0; i < itterations; i++)
            barnesHutSerial();

        print();

    }

    public static void barnesHutSerial() {
        Square root = new Square(0, -1, 0, 0.0, 0.0, 2*dimension, "root");
        root.bodies = bodies;
        BHtree.add(root);
        buildTree(root.bodies, root);
        for(Square s : BHtree) calculateCenterOfMass(s);
        for(int i = 0; i < bodies.size(); i++){
            calculateForce(i, 0);
            calculateSpeed(i);
            calculatePosition(i);
        }
    }

    public static void calculateSpeed(int index){
        Body b = bodies.get(index);
        b.vx = b.vx + b.fx / b.mass;
        b.vy = b.vy + b.fy / b.mass;
    }

    public static void calculatePosition(int index){
        Body b = bodies.get(index);
        b.x = b.x + b.vx;
        b.y = b.y + b.vy;
    }

    public static void calculateForce(int body_index, int square_index){
        Body b = bodies.get(body_index);
        Square s = BHtree.get(square_index);
        //if the node is external return
        if(s.bodies.size() == 1 || s.bodies.size() == 0) return;
        double dx = b.x - s.x_center;
        double dy = b.y - s.y_center;
        double distance = Math.sqrt(dx * dx + dy * dy);
        if(distance > b.minSideLength && !s.bodies.contains(b)){
            double force = G * b.mass * s.total_mass / (distance * distance);
            b.fx = b.fx + force * dx / distance;
            b.fy = b.fy + force * dy / distance;
        } else {
            if(BHtree.get(square_index*4 + 1) != null) calculateForce(body_index, square_index * 4 + 1);
            if(BHtree.get(square_index*4 + 2) != null) calculateForce(body_index, square_index * 4 + 2);
            if(BHtree.get(square_index*4 + 3) != null) calculateForce(body_index, square_index * 4 + 3);
            if(BHtree.get(square_index*4 + 4) != null) calculateForce(body_index, square_index * 4 + 4);
        }
    }

    public static void calculateCenterOfMass(Square s) {
        if(s.bodies.size() == 0 || s.bodies.size() == 1) return;

        double total_mass = 0.0;
        double x_center = 0.0;
        double y_center = 0.0;
        for(Body b : s.bodies){
            total_mass += b.mass;
            x_center += b.x * b.mass;
            y_center += b.y * b.mass;
        }
        x_center = x_center / total_mass;
        y_center = y_center / total_mass;
        s.x_center = x_center;
        s.y_center = y_center;
        s.total_mass = total_mass;
    }

    public static boolean isBodyInSquare(Body b, Square s){
        return b.x <= s.x_mid + s.length/2.0 && b.x >= s.x_mid - s.length/2.0 && b.y <= s.y_mid + s.length/2.0 && b.y >= s.y_mid - s.length/2.0;
    }

    public static void buildTree(ArrayList<Body> bodies, Square parent){

        Square top_left = new Square(parent.id * 4 + 1, parent.id, parent.level + 1, parent.x_mid - parent.length/4.0, parent.y_mid + parent.length/4.0, parent.length/2.0, "top_left");
        BHtree.add(top_left);
        for(Body b : bodies) {
            if(isBodyInSquare(b, top_left)) {
                b.minSideLength = top_left.length;
                top_left.bodies.add(b);
            }
        }
        if(top_left.bodies.size() > 1){
            buildTree(top_left.bodies, top_left);
        }

        Square top_right = new Square(parent.id * 4 + 2, parent.id, parent.level + 1, parent.x_mid + parent.length/4.0, parent.y_mid + parent.length/4.0, parent.length/2.0, "top_right");
        BHtree.add(top_right);
        for(Body b : bodies) {
            if(isBodyInSquare(b, top_right)){
                b.minSideLength = top_right.length;
                top_right.bodies.add(b);
            }
        }
        if(top_right.bodies.size() > 1){
            buildTree(top_right.bodies, top_right);
        }

        Square bottom_left = new Square(parent.id * 4 + 3, parent.id, parent.level + 1, parent.x_mid - parent.length/4.0, parent.y_mid - parent.length/4.0, parent.length/2.0, "bottom_left");
        BHtree.add(bottom_left);
        for(Body b : bodies) {
            if(isBodyInSquare(b, bottom_left)) {
                b.minSideLength = bottom_left.length;
                bottom_left.bodies.add(b);
            }
        }
        if(bottom_left.bodies.size() > 1){
            buildTree(bottom_left.bodies, bottom_left);
        }

        Square bottom_right = new Square(parent.id * 4 + 4, parent.id, parent.level + 1, parent.x_mid + parent.length/4.0, parent.y_mid - parent.length/4.0, parent.length/2.0, "bottom_right");
        BHtree.add(bottom_right);
        for(Body b : bodies) {
            if(isBodyInSquare(b, bottom_right)) {
                b.minSideLength = bottom_right.length;
                bottom_right.bodies.add(b);
            }
        }
        if(bottom_right.bodies.size() > 1){
            buildTree(bottom_right.bodies, bottom_right);
        }
    }

    public static void print() {
        try {
            FileWriter writer = new FileWriter("output.txt");
            for (Body b : bodies) {
                writer.write(b.toString() + "\n");
            }
            writer.close();
        } catch (IOException e) {
            System.out.println("An error occurred while writing to the file.");
            e.printStackTrace();
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

}

class BarnesHutThread extends Thread {
    int start;
    int end;

    BarnesHutThread(int start, int end){
        this.start = start;
        this.end = end;
    }

    public void run(){
        for(int i = start; i < end; i++){
            BarnesHut.calculateForce(i, 0);
            BarnesHut.calculateSpeed(i);
            BarnesHut.calculatePosition(i);
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
    double minSideLength;

    Body(int id, double x, double y, double vx, double vy, double mass, String s){
        this.id = id;
        this.x = x;
        this.y = y;
        this.mass = mass;
        this.vx = vx;
        this.vy = vy;
        this.s = s;
        this.fx = 0.0;
        this.fy = 0.0;
    }

    public String toString(){
        return x + " " + y + " " + vx + " " + vy + " " + mass + " " + s;
    }
}

class Square {
    int id;
    int parent_id;
    int level;
    double x_mid;
    double y_mid;
    double x_center;
    double y_center;
    double total_mass;
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
        this.x_center = 0.0;
        this.y_center = 0.0;
        this.total_mass = 0.0;
        this.length = length;
        this.location = location;
        this.bodies = new ArrayList<Body>();
        top_left = null;
        top_right = null;
        bottom_left = null;
        bottom_right = null;
    }
}