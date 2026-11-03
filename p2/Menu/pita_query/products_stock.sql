/*Stock: Se solicita al usuario un identificador de producto (productcode) y se devuelve el numero de unidades del producto en stock*/


SELECT quantityinstock FROM products WHERE productcode = ?;