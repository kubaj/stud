package ija.ija2016.homework1.cardpack;

/**
 * Created by xkulic03 on 2/24/17.
 */
public class Card {

    private Card.Color c;
    private int value;

    public Card(Card.Color c, int value) {
        this.c = c;
        this.value = value;
    }

    public Card.Color color() {
        return this.c;
    }

    public int value() {
        return this.value;
    }

    @Override
    public String toString() {
        String tValue;

        if (this.value == 1) {
            tValue = "A";
        } else if (this.value == 11) {
            tValue = "J";
        } else if (this.value == 12) {
            tValue = "Q";
        } else if (this.value == 13) {
            tValue = "K";
        } else {
            tValue = String.valueOf(this.value);
        }
        return tValue + "(" + this.c.toString() + ")";
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Card card = (Card) o;

        if (value != card.value) return false;
        return c == card.c;
    }

    @Override
    public int hashCode() {
        int result = c != null ? c.hashCode() : 0;
        result = 31 * result + value;
        return result;
    }

    public static enum Color {
        CLUBS, DIAMONDS, HEARTS, SPADES;

        @Override
        public String toString() {
            switch (this) {
                case CLUBS:
                    return "C";
                case HEARTS:
                    return "H";
                case DIAMONDS:
                    return "D";
                case SPADES:
                    return "S";
                default:
                    throw new IllegalArgumentException();
            }
        }
    }

    public static Card.Color valueOf(java.lang.String name) {

        if (name == null) {
            throw new NullPointerException();
        }

        switch (name) {
            case "S":
                return Color.SPADES;
            case "D":
                return Color.DIAMONDS;
            case "H":
                return Color.HEARTS;
            case "C":
                return Color.CLUBS;
            default:
                throw new IllegalArgumentException();
        }
    }


}
